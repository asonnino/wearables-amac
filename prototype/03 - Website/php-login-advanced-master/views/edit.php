<?php include('_header.php'); ?>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" href="/style/main.css"/>
        <link rel="stylesheet" href="/style/form.css"/>
        <link rel="stylesheet" href="/style/table.css"/>
        <link rel="stylesheet" href="/style/login.css"/>
        <link rel="stylesheet" href="/style/edit.css"/>
        <script type="text/javascript" src="http://code.jquery.com/jquery-latest.min.js"></script>
        <title>Account Details</title>
    </head>

<script type="text/javascript">
$(document).ready(function(){
    // variables
    var fadeInSpeed = 1000;

    // listeners
    $("#username_button").click(function(){
        $("#username_field").hide(); $("#username_field_edit").fadeIn(fadeInSpeed); $("#user_name").focus();
    });
    $("#email_button").click(function(){
        $("#email_field").hide(); $("#email_field_edit").fadeIn(fadeInSpeed); $("#email").focus();
    });
    $("#first_name_button").click(function(){
        $("#first_name_field").hide(); $("#first_name_field_edit").fadeIn(fadeInSpeed); $("#first_name").focus();
    });
    $("#last_name_button").click(function(){
        $("#last_name_field").hide(); $("#last_name_field_edit").fadeIn(fadeInSpeed); $("#last_name").focus();
    });
    $("#password_button").click(function(){
        $("#password_field").hide(); $("#password_field_edit").fadeIn(fadeInSpeed); $("#password").focus();

        $("#new_password_label").fadeIn(fadeInSpeed); 
        $("#new_password_field_edit").fadeIn(fadeInSpeed);
        $("#repeat_password_field_edit").fadeIn(fadeInSpeed);
        $("#button_password_field_edit").fadeIn(fadeInSpeed);
        
    });

    // init status
    inti_status();
});

function inti_status() {
    $("#username_field_edit").hide(); $("#username_field").show();
    $("#email_field_edit").hide(); $("#email_field").show();
    $("#first_name_field_edit").hide(); $("#first_name_field").show();
    $("#last_name_field_edit").hide(); $("#last_name_field").show(); 
    $("#password_field_edit").hide(); $("#password_field").show();

    $("#new_password_label").hide(); 
    $("#new_password_field_edit").hide();
    $("#repeat_password_field_edit").hide();
    $("#button_password_field_edit").hide();
    
}

</script>

    <body>
    <?php include '../menu/menu.php';?>

        <header>
            <h1><div class="bottom_50">Account Details</div></h1>
        </header>

        <!-- edit form for username / this form uses HTML5 attributes, like "required" and type="email" -->
		
		<main class="edit">
		<table>
			<tr>
				<td>
                <label for="user_name">Username:</label>
                </td>
                <td>
                <label for="user_email">Email:</label>
                </td>
			</tr>
			<tr>
                <td class="bottom_30">
                <form method="post" action="edit.php" name="user_edit_form_name">
                <div class="field" id="username_field">
                    <?php echo htmlspecialchars($_SESSION['user_name']); ?>
                    <input type="button" value="Change" class="small_button" id="username_button"/> 
                </div>
                <div class="field_edit" id="username_field_edit">
                	<input type='text' name='user_name' id='user_name' class='standard_input' 
                    placeholder='   Public Username' pattern="[a-zA-Z0-9]{2,64}" required /> 
                    <input type="submit" name="user_edit_submit_name" value="Set" class="small_button" />
                </div>
                </form> 
                </td>
                <td class="bottom_30">
                <form method="post" action="edit.php" name="user_edit_form_email">
                <div class="field" id="email_field">
                    <?php echo htmlspecialchars($_SESSION['user_email']); ?>
                    <input type="button" value="Change" class="small_button" id="email_button"/> 
                </div>
                <div class="field_edit" id="email_field_edit">
                	<input type='email' name='user_email' id='user_email' class='standard_input' 
                    placeholder='   Email' required /> 
                    <input type="submit" name="user_edit_submit_email" value="Set" class="small_button" />
                </div>
                </form> 
                </td>	
            </tr>


            <tr>
            	<td colspan="2" class="bottom_30">
            		 <hr/>
            	</td>
            </tr>


            <form method="post" action="edit.php" name="user_edit_form_password">
			<tr>
				<td>
                <label for="user_password_old">Password:</label>
                </td>
                <td>
                <label for="user_password_new" id="new_password_label">New Password:</label>
                </td>
			</tr>
			<tr>
				<td class="bottom_30">
                <div class="field" id="password_field">
                    *************
                    <input type="button" value="Change" class="small_button" id="password_button"/> 
                </div>
                <div class="field_edit" id="password_field_edit">
                	<input type='password' name='user_password_old' id='user_password_old' class='standard_input' placeholder='   Old Password' required autocomplete="off" /> 
                </div>
                <td class="bottom_30">
                <div>
					<!-- EMPTY -->
				</div>
                <div class="field_edit" id="new_password_field_edit">
                	<input type='password' name='user_password_new' id='user_password_new' class='standard_input' placeholder='   New Password' required autocomplete="off" /> 
                </div>	               
            </tr>
            <tr>
				<td class="bottom_30">
				<div>
					<!-- EMPTY -->
				</div>
                <div class="password_button" id="button_password_field_edit">
                	<input type="submit" name="user_edit_submit_password" value="Update Password" class="small_button" /> 
                </div>
                <td class="bottom_30">
                <div>
					<!-- EMPTY -->
				</div>
                <div class="field_edit" id="repeat_password_field_edit">
                	<input type='password' name='user_password_repeat' id='user_password_repeat' class='standard_input' placeholder='   Repeat New Password' required autocomplete="off" /> 
                </div>	               
            </tr>
            </form>


            <tr>
        		<td colspan="2" class="bottom_30">
            		 <hr/>
            	</td>
            </tr>
            

            <tr>
            	<td colspan="2" style="text-align: center; font-size: small;">
					<!-- backlink -->
				<a href="index.php"><?php echo WORDING_BACK_TO_LOGIN; ?></a>
            	</td>
            </tr>
            <tr>
                <td colspan="2">
                <div class=error_messages>
                    <?php echo $_SESSION['login_error']; ?>
                </div>
                </td>
            </tr>
		</table>
        </main>

    <?php include '../copyright.php';?>
    </body>
</html>



<?php
// handle user's errors



?>

<?php 
    // the footer clears the error and messages session's varibales
    include('_footer.php'); 
?>
