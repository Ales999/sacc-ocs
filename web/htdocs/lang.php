<?php
setcookie('lang',$_GET['lang']);
Header("Location: ".base64_decode($_GET['query']),time()+60*60*24*60);
setcookie('query',"");
?>
