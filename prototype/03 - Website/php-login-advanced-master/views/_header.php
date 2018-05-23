<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <!--
    <style type="text/css">
        /* just for the demo */
        body {
            font-family: Arial, sans-serif;
            font-size: 12px;
            margin: 10px;
        }
        label {
            position: relative;
            vertical-align: middle;
            bottom: 1px;
        }
        input[type=text],
        input[type=password],
        input[type=submit],
        input[type=email] {
            display: block;
            margin-bottom: 15px;
        }
        input[type=checkbox] {
            margin-bottom: 15px;
        }
    </style>
    -->
</head>
<body>

<?php
session_start();
// show potential errors / feedback (from login object)
if (isset($login)) {
    if ($login->errors) {
        foreach ($login->errors as $error) {
            $_SESSION['login_error'] = $error;
            //echo $error; // debug
        }
    }
    if ($login->messages) {
        foreach ($login->messages as $message) {
            $_SESSION['login_message'] = $message;
            //echo $message; // debug
        }
    }
}
?>

<?php
// show potential errors / feedback (from registration object)
if (isset($registration)) {
    if ($registration->errors) {
        foreach ($registration->errors as $error) {
            $_SESSION['register_error'] = $error;
            //echo $error; // debug
        }
    }
    if ($registration->messages) {
        foreach ($registration->messages as $message) {
            $_SESSION['register_message'] = $message;
            //echo $message; // debug
        }
    }
}
?>
