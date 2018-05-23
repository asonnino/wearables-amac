<?php include('_header.php'); ?>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" href="/style/main.css"/>
        <link rel="stylesheet" href="/style/form.css"/>
        <link rel="stylesheet" href="/style/table.css"/>
        <link rel="stylesheet" href="/style/login.css"/>
        <title>Login</title>
    </head>

    <body>
    <?php include '../menu/menu.php';?>


<?php  if ($_SESSION['login_message'] == MESSAGE_PASSWORD_CHANGED_SUCCESSFULLY) { ?>


        <header>
            <h1><div class="bottom_50">Password Changed Successfully</div></h1>
        </header>

        <main>
        
            <table>
                <tr>
                    <td style="text-align: center;">
                    <?php echo (MESSAGE_PASSWORD_CHANGED_SUCCESSFULLY); ?>
                    <div class="top_10">
                        <a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
                    </div>
                    </td>
                </tr>
            </table>

            <?php 
                // reset field 
                $_SESSION['login_message'] = false;
            ?>

<?php } 

else { ?>


        <header>
            <h1><div class="bottom_50">Login</div></h1>
        </header>

        <main class="login">

			<form id="form" method="post" action="index.php" name="loginform">
            <table> 
                <tr><td colspan="2">
                    <label for="user_name"> Username or Email: </label>
                </td></tr>
                <tr><td colspan="2" class="bottom_10">
                	<input id="user_name" type="text" name="user_name" class="standard_input" placeholder="   Username" required />
                </td></tr>

                <tr><td colspan="2">
                    <label for="user_password"><?php echo WORDING_PASSWORD; ?></label>
                </td></tr>
                <tr><td colspan="2">
                    <input id="user_password" type="password" name="user_password" autocomplete="off" class="standard_input" placeholder="   Password" required />
                    <input type="image" src="/images/access.png" alt="Submit Form" 
                    value="<?php echo WORDING_LOGIN; ?>" name="login" class="submit_button"/>
                </td></tr>
                </tr>

                <tr>
                    <td class="create_account">
                        <a href="register.php"><?php echo WORDING_REGISTER_NEW_ACCOUNT; ?></a>  
                    </td>
                    <td class="forgot_password">              
                        <a href="password_reset.php"><?php echo WORDING_FORGOT_MY_PASSWORD; ?></a>
                    </td>
                </tr>

                <tr><td colspan="2" class=remember_me>
                    <input type="checkbox" name="user_rememberme" id="remember_me"/>
                    <label for="user_rememberme"><?php echo WORDING_REMEMBER_ME; ?></label>
                </td></tr>

                <tr><td colspan="2">
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
                </td></tr>

                </table>      
            </form>

<?php } ?>

        </main>

    <?php include '../copyright.php';?>
    </body>
</html>


<?php
switch($_SESSION['login_error']){
    case false : // reset form style
        echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form";'
        ,'</script>';
        break;
    case MESSAGE_USERNAME_EMPTY :
        echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_username";'
        ,'</script>';
        break;
    case MESSAGE_PASSWORD_EMPTY :
        echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error_password";'
        ,'</script>';
        break;
    default :   
        echo '<script type="text/javascript">'
        , 'document.getElementById("form").className = "form_error";'
        ,'</script>';
}

?>

<?php 
    // the footer clears the error and messages session's varibales
    include('_footer.php'); 
?>