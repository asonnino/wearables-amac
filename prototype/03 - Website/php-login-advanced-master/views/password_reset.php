<?php include('_header.php'); ?>


<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="/style/main.css"/>
        <link rel="stylesheet" href="/style/form.css"/>
        <link rel="stylesheet" href="/style/table.css"/>
        <title>Password Reset</title>
    </head>

    <body>
    <?php include '../menu/menu.php';?>


    <header>
        <h1><div class="bottom_30">Password Reset</div></h1>
    </header>

    <style type="text/css">
        table {
            text-align: center;
        }
    </style>

    <main class="password_reset">
<?php  if ($login->passwordResetLinkIsValid() == true) { ?>

        <form method="post" id="form" action="password_reset.php" name="new_password_form">

        <input type='hidden' name='user_name' value='<?php echo htmlspecialchars($_GET['user_name']); ?>' />
        <input type='hidden' name='user_password_reset_hash' value='<?php echo htmlspecialchars($_GET['verification_code']); ?>' />

        <table> 
            <tr>
                <td>
                <label for="user_password_new"><?php echo WORDING_NEW_PASSWORD; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_10">
                <input id="user_password_new" type="password" name="user_password_new" pattern=".{6,}" class="standard_input" placeholder="   New Password" required autocomplete="off" />
                </td>
            </tr>
            <tr>
                <td>
                <label for="user_password_repeat"><?php echo WORDING_NEW_PASSWORD_REPEAT; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_50">
                <input id="user_password_repeat" type="password" name="user_password_repeat" pattern=".{6,}" class="standard_input" placeholder="   Confirm New Password" required autocomplete="off" />
                </td>
            </tr>
            <tr>
                <td class="button">
                <input 
                    type="submit" 
                    name="submit_new_password" 
                    class="standard_button" 
                    value="<?php echo WORDING_SUBMIT_NEW_PASSWORD; ?>" 
                />
                <div class="top_10">
                    <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                </div>
                </td>
            </tr>
            <tr>
                <td colspan="2">
                <div class=error_messages>
                    <?php 
                        if (isset($login)) {
                            if ($login->errors) {
                                foreach ($login->errors as $error) {
                                    echo $error;
                                }
                            }
                        }
                    ?>
                </div>
                </td>
            </tr>
        </table>  
        </form>

<?php } 

// ack success in sending back email
else if ($_SESSION['login_message'] == MESSAGE_PASSWORD_RESET_MAIL_SUCCESSFULLY_SENT) { ?>

        <table>
            <tr>
                <td>
                <?php echo (MESSAGE_PASSWORD_RESET_MAIL_SUCCESSFULLY_SENT); ?>
                <div class="top_10">
                    <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                </div>
                </td>
            </tr>
        </table>

<?php } 

// no data from a password-reset-mail has been provided, so we simply show the request-a-password-reset form
else { ?>
        
        <form method="post" id="form" action="password_reset.php" name="password_reset_form">
        <table> 
            <tr>
                <td class="bottom_10">
                <label for="user_name"><?php echo WORDING_REQUEST_PASSWORD_RESET; ?></label>
                </td>
            </tr>
            <tr>
                <td class="bottom_50">
                <input id="user_name" type="text" name="user_name" class="standard_input" placeholder="   Public Username" required />
                </td>
            </tr>
            <tr>
                <td class="button">
                <input 
                    type="submit" 
                    name="request_password_reset" 
                    class="standard_button" 
                    value="<?php echo WORDING_RESET_PASSWORD; ?>" 
                />
                <div class="top_10">
                    <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                </div>
                </td>
            </tr>
            <tr>
                <td colspan="2">
                <div class=error_messages>
                    <?php 
                        if (isset($login)) {
                            if ($login->errors) {
                                foreach ($login->errors as $error) {
                                    echo $error;
                                }
                                echo " Please restart the procedure.";
                            }
                        }
                    ?> 
                </div>
                </td>
            </tr>
        </table>      
        </form>

<?php } ?>

    </main>

    <?php include '../copyright.php';?>
    </body>
</html>


<?php
if (
    ($_SESSION['login_error'] == MESSAGE_USERNAME_EMPTY)      ||
    ($_SESSION['login_error'] == MESSAGE_USER_DOES_NOT_EXIST) 
) {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_username";'
        ,'</script>';
}
else {
    echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form";'
        ,'</script>';  
}
?>


<?php 
    // the footer clears the error and messages session's varibales
    include('_footer.php'); 
?>