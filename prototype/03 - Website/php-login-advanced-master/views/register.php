<?php include('_header.php'); ?>

<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="/style/main.css"/>
        <link rel="stylesheet" href="/style/form.css"/>
        <link rel="stylesheet" href="/style/table.css"/>
        <title>Register</title>
    </head>

    <style type="text/css">
        .register table {
            border-spacing: 50px 0;
        }
    </style>


    <body>
    <?php include '../menu/menu.php';?>


    <header>
        <h1><div class="bottom_30">Create Account</div></h1>
    </header>

    <main class="register">
    
<!-- show registration form, but only if we didn't submit already -->
<?php  if (!$registration->registration_successful && !$registration->verification_successful) { ?>

        <form method="post" id="form" action="register.php" name="registerform">
        <table> 
            <tr>
                <td>
                <label for="user_name"><?php echo WORDING_REGISTRATION_USERNAME; ?></label>
                </td>
                <td>
                <label for="user_email"><?php echo WORDING_REGISTRATION_EMAIL; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_30">
                <input id="user_name" type="text" pattern="[a-zA-Z0-9]{2,64}" name="user_name" class="standard_input" placeholder="   Public Username" required />
                </td>
                <td class="bottom_30">
                <input id="user_email" type="email" name="user_email" class="standard_input" 
                    placeholder="   Email Address" required />
                </td>
            </tr>

            <tr>
                <td>
                <label for="first_name">First Name:</label>
                </td>
                <td>
                <label for="user_password_new"><?php echo WORDING_REGISTRATION_PASSWORD; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_10">
                <input type="text" name="first_name" id="first_name" class="standard_input" 
                    placeholder="   First name" required/>
                </td>
                <td class="bottom_10">
                <input id="user_password_new" type="password" name="user_password_new" pattern=".{6,}" required autocomplete="off" class="standard_input" placeholder="   Password" />
                </td>
            </tr>

            <tr>
                <td>
                <label for="last_name">Last Name:</label>
                </td>
                <td>
                <label for="user_password_repeat"><?php echo WORDING_REGISTRATION_PASSWORD_REPEAT; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_30">
                <input type="text" name="last_name" id="last_name" class="standard_input" 
                    placeholder="   Last name" required/>
                </td>
                <td class="bottom_30">
                <input id="user_password_repeat" type="password" name="user_password_repeat" pattern=".{6,}" required autocomplete="off" class="standard_input" placeholder="   Retype Password" />
                </td>
            </tr>
            <tr> 
                <td colspan="2" class="bottom_50" style="text-align: center;">
                <img src="tools/showCaptcha.php" alt="captcha" />
                <label><?php //echo WORDING_REGISTRATION_CAPTCHA; ?></label>
                <input id="captcha" type="text" name="captcha" class="small_input" placeholder="    Enter Captcha" required />
                </td>

            <tr>
                <td colspan="2" class="button">
                <input 
                    type="submit" 
                    name="register" 
                    class="standard_button" 
                    value="<?php echo WORDING_REGISTER; ?>" 
                />
                <div class="top_10">
                    <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                </div>
                </td>
            </tr>
            <tr>
                <td colspan="2">
                <div class=error_messages>
                    <?php echo $_SESSION['register_error']; ?>
                </div>
                </td>
            </tr>
        </table>      
        </form>

<?php } 

// display feedback on registration
else { ?>

        <table>
            <tr>
                <td style="text-align:center;">
                <div>
                    <?php 
                        if (isset($registration)) {
                            if ($registration->messages) {
                                foreach ($registration->messages as $message) {
                                    echo $message;
                                }
                            }
                        }
                    ?>
                </div>
                <div style="color:red; text-size:small;">
                   <?php 
                        if (isset($registration)) {
                            if ($registration->errors) {
                                foreach ($registration->errors as $error) {
                                    echo $error;
                                }
                            }
                        }
                   ?>

                </div>
                <div class="top_10">
                    <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                </div>
                </td>
            </tr>
        </table>
<?php } ?>

    </main>

    <?php include '../copyright.php';?>
    </body>
</html>


<?php
if ($_SESSION['register_error'] == false) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form";'
        ,'</script>';  
}
else if (
    ($_SESSION['register_error'] == MESSAGE_USERNAME_EMPTY)      ||
    ($_SESSION['register_error'] == MESSAGE_USERNAME_BAD_LENGTH) ||
    ($_SESSION['register_error'] == MESSAGE_USERNAME_EXISTS)     ||
    ($_SESSION['register_error'] == MESSAGE_USERNAME_INVALID)
) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_username";'
        ,'</script>';
}
else if ($_SESSION['register_error'] == MESSAGE_CAPTCHA_WRONG) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_captcha";'
        ,'</script>';
}
else if (
    ($_SESSION['register_error'] == MESSAGE_PASSWORD_EMPTY)       ||
    ($_SESSION['register_error'] == MESSAGE_PASSWORD_BAD_CONFIRM) ||
    ($_SESSION['register_error'] == MESSAGE_PASSWORD_TOO_SHORT)
) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_password";'
        ,'</script>';
}
else if (
    ($_SESSION['register_error'] == MESSAGE_EMAIL_EMPTY)    ||
    ($_SESSION['register_error'] == MESSAGE_EMAIL_INVALID)  ||
    ($_SESSION['register_error'] == MESSAGE_EMAIL_TOO_LONG) 
) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_email";'
        ,'</script>';
}
else {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error";'
        ,'</script>';
}
?>

<?php 
    // the footer clears the error and messages session's varibales
    include('_footer.php'); 
?>