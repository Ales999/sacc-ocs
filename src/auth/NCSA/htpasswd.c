/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * Portions of this software are based upon public domain software
 * originally written at the National Center for Supercomputing Applications,
 * University of Illinois, Urbana-Champaign.
 */

/******************************************************************************
 ******************************************************************************
 * NOTE! This program is not safe as a setuid executable!  Do not make it
 * setuid!
 ******************************************************************************
 *****************************************************************************/
/*
 * htpasswd.c: simple program for manipulating password file for
 * the Apache HTTP server
 * 
 * Originally by Rob McCool
 *
 * Exit values:
 *  0: Success
 *  1: Failure; file access/permission problem
 *  2: Failure; command line syntax problem (usage message issued)
 *  3: Failure; password verification failure
 *  4: Failure; operation interrupted (such as with CTRL/C)
 *  5: Failure; buffer would overflow (username, filename, or computed
 *     record too long)
 *  6: Failure; username contains illegal or reserved characters
 */
#include "config.h"
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if HAVE_CRYPT_H
#include <crypt.h>
#endif

#include <signal.h>
#include <errno.h>

#ifndef CHARSET_EBCDIC
#define LF 10
#define CR 13
#else /*CHARSET_EBCDIC*/
#define LF '\n'
#define CR '\r'
#endif /*CHARSET_EBCDIC*/

#define MAX_STRING_LEN 256
#define ALG_PLAIN 0
#define ALG_CRYPT 1
#define ALG_APMD5 2
#define ALG_APSHA 3 

#define ERR_FILEPERM 1
#define ERR_SYNTAX 2
#define ERR_PWMISMATCH 3
#define ERR_INTERRUPTED 4
#define ERR_OVERFLOW 5
#define ERR_BADUSER 6

void ap_to64(char *s, unsigned long v, int n)
{
    static unsigned char itoa64[] =         /* 0 ... 63 => ASCII - 64 */
	"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    while (--n >= 0) {
	*s++ = itoa64[v&0x3f];
	v >>= 6;
    }
}

/*
 * Get a line of input from the user, not including any terminating
 * newline.
 */
static int getline(char *s, int n, FILE *f)
{
    register int i = 0;

    while (1) {
	s[i] = (char) fgetc(f);

	if (s[i] == CR) {
	    s[i] = fgetc(f);
	}

	if ((s[i] == 0x4) || (s[i] == LF) || (i == (n - 1))) {
	    s[i] = '\0';
	    return (feof(f) ? 1 : 0);
	}
	++i;
    }
}

char* ap_cpystrn(char *dst, const char *src, size_t dst_size)
{

    char *d, *end;

    if (!dst_size)
        return (dst);

    d = dst;
    end = dst + dst_size - 1;

    for (; d < end; ++d, ++src) {
	if (!(*d = *src)) {
	    return (d);
	}
    }

    *d = '\0';	/* always null terminate */

    return (d);
}

/*
 * This needs to be declared statically so the signal handler can
 * access it.
 */
static char *tempfilename;
/*
 * If our platform knows about the tmpnam() external buffer size, create
 * a buffer to pass in.  This is needed in a threaded environment, or
 * one that thinks it is (like HP-UX).
 */
#ifdef L_tmpnam
static char tname_buf[L_tmpnam];
#else
static char *tname_buf = NULL;
#endif

static void putline(FILE *f, char *l)
{
    int x;

    for (x = 0; l[x]; x++) {
	fputc(l[x], f);
    }
    fputc('\n', f);
}

/*
 * Make a password record from the given information.  A zero return
 * indicates success; failure means that the output buffer contains an
 * error message instead.
 */
static int mkrecord(char *user, char *record, size_t rlen, char *passwd,
		    int alg)
{
    char *pw;
    char cpw[120];
    char pwin[MAX_STRING_LEN];
    char pwv[MAX_STRING_LEN];
    char salt[9];

    if (passwd != NULL) {
	pw = passwd;
    }
    switch (alg) {

    case ALG_CRYPT:
    default:
        (void) srand((int) time((time_t *) NULL));
        ap_to64(&salt[0], rand(), 8);
        salt[8] = '\0';

	ap_cpystrn(cpw, (char *)crypt(pw, salt), sizeof(cpw) - 1);
	break;
    }
    memset(pw, '\0', strlen(pw));

    /*
     * Check to see if the buffer is large enough to hold the username,
     * hash, and delimiters.
     */
    if ((strlen(user) + 1 + strlen(cpw)) > (rlen - 1)) {
	ap_cpystrn(record, "resultant record too long", (rlen - 1));
	return ERR_OVERFLOW;
    }
    strcpy(record, user);
    strcat(record, ":");
    strcat(record, cpw);
    return 0;
}

static int usage(void)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\thtpasswd -b[cmdps] passwordfile username password\n\n");
    fprintf(stderr, "\thtpasswd -nb[mdps] username password\n");
    fprintf(stderr, " -c  Create a new file.\n");
    fprintf(stderr, " -n  Don't update file; display results on stdout.\n");
    fprintf(stderr, " -d  Force CRYPT encryption of the password"
#if (!(defined(WIN32) || defined(TPF) || defined(NETWARE)))
	    " (default)"
#endif
	    ".\n");
    fprintf(stderr, " -b  Use the password from the command line rather "
	    "than prompting for it.\n");
    return ERR_SYNTAX;
}

static void interrupted(void)
{
    fprintf(stderr, "Interrupted.\n");
    if (tempfilename != NULL) {
	unlink(tempfilename);
    }
    exit(ERR_INTERRUPTED);
}

/*
 * Check to see if the specified file can be opened for the given
 * access.
 */
static int accessible(char *fname, char *mode)
{
    FILE *s;

    s = fopen(fname, mode);
    if (s == NULL) {
	return 0;
    }
    fclose(s);
    return 1;
}

/*
 * Return true if a file is readable.
 */
static int readable(char *fname)
{
    return accessible(fname, "r");
}

/*
 * Return true if the specified file can be opened for write access.
 */
static int writable(char *fname)
{
    return accessible(fname, "a");
}

/*
 * Return true if the named file exists, regardless of permissions.
 */
static int exists(char *fname)
{
    struct stat sbuf;
    int check;

    check = stat(fname, &sbuf);
    return ((check == -1) && (errno == ENOENT)) ? 0 : 1;
}

/*
 * Copy from the current position of one file to the current position
 * of another.
 */
static void copy_file(FILE *target, FILE *source)
{
    static char line[MAX_STRING_LEN];

    while (fgets(line, sizeof(line), source) != NULL) {
	fputs(line, target);
    }
}

/*
 * Let's do it.  We end up doing a lot of file opening and closing,
 * but what do we care?  This application isn't run constantly.
 */
int main(int argc, char *argv[])
{
    FILE *ftemp = NULL;
    FILE *fpw = NULL;
    char user[MAX_STRING_LEN];
    char password[MAX_STRING_LEN];
    char record[MAX_STRING_LEN];
    char line[MAX_STRING_LEN];
    char pwfilename[MAX_STRING_LEN];
    char *arg;
    int found = 0;
    int alg = ALG_CRYPT;
    int newfile = 0;
    int nofile = 0;
    int noninteractive = 0;
    int i;
    int args_left = 2;

    tempfilename = NULL;
    signal(SIGINT, (void (*)(int)) interrupted);

    /*
     * Preliminary check to make sure they provided at least
     * three arguments, we'll do better argument checking as 
     * we parse the command line.
     */
    if (argc < 3) {
	return usage();
    }

    /*
     * Go through the argument list and pick out any options.  They
     * have to precede any other arguments.
     */
    for (i = 1; i < argc; i++) {
	arg = argv[i];
	if (*arg != '-') {
	    break;
	}
	while (*++arg != '\0') {
	    if (*arg == 'c') {
		newfile++;
	    }
	    else if (*arg == 'n') {
		nofile++;
		args_left--;
	    }
	    else if (*arg == 'm') {
		alg = ALG_APMD5;
	    }
	    else if (*arg == 's') {
		alg = ALG_APSHA;
	    }
	    else if (*arg == 'p') {
		alg = ALG_PLAIN;
	    }
	    else if (*arg == 'd') {
		alg = ALG_CRYPT;
	    }
	    else if (*arg == 'b') {
		noninteractive++;
		args_left++;
	    }
	    else {
		return usage();
	    }
	}
    }

    /*
     * Make sure we still have exactly the right number of arguments left
     * (the filename, the username, and possibly the password if -b was
     * specified).
     */
    if ((argc - i) != args_left) {
	return usage();
    }
    if (newfile && nofile) {
	fprintf(stderr, "%s: -c and -n options conflict\n", argv[0]);
	return ERR_SYNTAX;
    }
    if (nofile) {
	i--;
    }
    else {
	if (strlen(argv[i]) > (sizeof(pwfilename) - 1)) {
	    fprintf(stderr, "%s: filename too long\n", argv[0]);
	    return ERR_OVERFLOW;
	}
	strcpy(pwfilename, argv[i]);
	if (strlen(argv[i + 1]) > (sizeof(user) - 1)) {
	    fprintf(stderr, "%s: username too long (>%lu)\n", argv[0],
		    (unsigned long)(sizeof(user) - 1));
	    return ERR_OVERFLOW;
	}
    }
    strcpy(user, argv[i + 1]);
    if ((arg = strchr(user, ':')) != NULL) {
	fprintf(stderr, "%s: username contains illegal character '%c'\n",
		argv[0], *arg);
	return ERR_BADUSER;
    }
    if (noninteractive) {
	if (strlen(argv[i + 2]) > (sizeof(password) - 1)) {
	    fprintf(stderr, "%s: password too long (>%lu)\n", argv[0],
		    (unsigned long)(sizeof(password) - 1));
	    return ERR_OVERFLOW;
	}
	strcpy(password, argv[i + 2]);
    }

#if (!(defined(WIN32) || defined(TPF) || defined(NETWARE)))
    if (alg == ALG_PLAIN) {
	fprintf(stderr,"Warning: storing passwords as plain text might "
		"just not work on this platform.\n");
    }
#endif
    if (! nofile) {
	/*
	 * Only do the file checks if we're supposed to frob it.
	 *
	 * Verify that the file exists if -c was omitted.  We give a special
	 * message if it doesn't.
	 */
	if ((! newfile) && (! exists(pwfilename))) {
	    fprintf(stderr,
		    "%s: cannot modify file %s; use '-c' to create it\n",
		    argv[0], pwfilename);
	    perror("fopen");
	    exit(ERR_FILEPERM);
	}
	/*
	 * Verify that we can read the existing file in the case of an update
	 * to it (rather than creation of a new one).
	 */
	if ((! newfile) && (! readable(pwfilename))) {
	    fprintf(stderr, "%s: cannot open file %s for read access\n",
		    argv[0], pwfilename);
	    perror("fopen");
	    exit(ERR_FILEPERM);
	}
	/*
	 * Now check to see if we can preserve an existing file in case
	 * of password verification errors on a -c operation.
	 */
	if (newfile && exists(pwfilename) && (! readable(pwfilename))) {
	    fprintf(stderr, "%s: cannot open file %s for read access\n"
		    "%s: existing auth data would be lost on "
		    "password mismatch",
		    argv[0], pwfilename, argv[0]);
	    perror("fopen");
	    exit(ERR_FILEPERM);
	}
	/*
	 * Now verify that the file is writable!
	 */
	if (! writable(pwfilename)) {
	    fprintf(stderr, "%s: cannot open file %s for write access\n",
		    argv[0], pwfilename);
	    perror("fopen");
	    exit(ERR_FILEPERM);
	}
    }

    /*
     * All the file access checks (if any) have been made.  Time to go to work;
     * try to create the record for the username in question.  If that
     * fails, there's no need to waste any time on file manipulations.
     * Any error message text is returned in the record buffer, since
     * the mkrecord() routine doesn't have access to argv[].
     */
    i = mkrecord(user, record, sizeof(record) - 1,
		 noninteractive ? password : NULL,
		 alg);
    if (i != 0) {
	fprintf(stderr, "%s: %s\n", argv[0], record);
	exit(i);
    }
    if (nofile) {
	printf("%s\n", record);
	exit(0);
    }

    /*
     * We can access the files the right way, and we have a record
     * to add or update.  Let's do it..
     */
    errno = 0;
    tempfilename = tmpnam(tname_buf);
    if ((tempfilename == NULL) || (*tempfilename == '\0')) {
	fprintf(stderr, "%s: unable to generate temporary filename\n",
		argv[0]);
	if (errno == 0) {
	    errno = ENOENT;
	}
	perror("tmpnam");
	exit(ERR_FILEPERM);
    }
    ftemp = fopen(tempfilename, "w+");
    if (ftemp == NULL) {
	fprintf(stderr, "%s: unable to create temporary file '%s'\n", argv[0],
		tempfilename);
	perror("fopen");
	exit(ERR_FILEPERM);
    }
    /*
     * If we're not creating a new file, copy records from the existing
     * one to the temporary file until we find the specified user.
     */
    if (! newfile) {
	char scratch[MAX_STRING_LEN];

	fpw = fopen(pwfilename, "r");
	while (! (getline(line, sizeof(line), fpw))) {
	    char *colon;

	    if ((line[0] == '#') || (line[0] == '\0')) {
		putline(ftemp, line);
		continue;
	    }
	    strcpy(scratch, line);
	    /*
	     * See if this is our user.
	     */
	    colon = strchr(scratch, ':');
	    if (colon != NULL) {
		*colon = '\0';
	    }
	    if (strcmp(user, scratch) != 0) {
		putline(ftemp, line);
		continue;
	    }
	    found++;
	    break;
	}
    }
    if (found) {
	fprintf(stderr, "Updating ");
    }
    else {
	fprintf(stderr, "Adding ");
    }
    fprintf(stderr, "password for user %s\n", user);
    /*
     * Now add the user record we created.
     */
    putline(ftemp, record);
    /*
     * If we're updating an existing file, there may be additional
     * records beyond the one we're updating, so copy them.
     */
    if (! newfile) {
	copy_file(ftemp, fpw);
	fclose(fpw);
    }
    /*
     * The temporary file now contains the information that should be
     * in the actual password file.  Close the open files, re-open them
     * in the appropriate mode, and copy them file to the real one.
     */
    fclose(ftemp);
    fpw = fopen(pwfilename, "w+");
    ftemp = fopen(tempfilename, "r");
    copy_file(fpw, ftemp);
    fclose(fpw);
    fclose(ftemp);
    unlink(tempfilename);
    return 0;
}
