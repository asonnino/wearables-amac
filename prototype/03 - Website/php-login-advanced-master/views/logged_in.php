<?php include('_header.php'); ?>


<?php 
// load the credentials class
require_once('../classes/Credentials.php');

// create a credential object
$credentials = new Credentials();
?>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" href="/style/main.css"/>
        <link rel="stylesheet" href="/style/form.css"/>
        <link rel="stylesheet" href="/style/table.css"/>
        <link rel="stylesheet" href="/style/logged_in.css"/>
        <title>Login</title>
    </head>


    <body>
    <?php include '../menu/menu.php';?>

        <header>
            <h1 class="top_30">
            	<div class="bottom_50">
            		<?php echo("Welcome " .htmlspecialchars($_SESSION['user_name']). " !"); ?>
    			</div>
        	</h1>
        </header>

        	<main class="main">

                
<!-- if admin is connected -->
<?php  if ($_SESSION['is_admin'] == 1) { ?>
    <?php

        // get user's info
        $user_attributes = $credentials->getAllUserAttributes();

        echo "<table class='cred_table'style='width:700px;'>";
            echo "<tr>";
                echo "<th class='user_th'>User</th>";
                echo "<th>Attributes</th>";
                echo "<th>Admin</th>";
                echo "<th>Edit</th>";
            echo "</tr>";

            echo "<tr>";
                echo "<td class='empty_line'></td>";
            echo "</tr>";

        // loop over users
        for ($i = 0; $i < count($user_attributes); $i++) {
            // get user's data
            $user_data = $login->getUserDataByID($user_attributes[$i][0]);

            // print table           
            echo "<tr>";

                echo "<td>";
                    echo "<div class=name>";
                        echo $user_data->user_name;
                    echo "</div>";
                    echo "<div class=email>";
                        echo $user_data->user_email;
                    echo "</div>";
                echo "</td>";

                echo "<td class=attr>";
                    echo "<ul>";
                    for ($j = 1; $j < (count($user_attributes[$i]) / 2); $j++) { // skip user_id
                        echo "<li>";
                            echo "m" .$j. " = " .$user_attributes[$i][$j];
                        echo "</li>";

                    }
                    echo "</ul>";
                echo "</td>";

                echo "<td class='role'>";
                    if ($user_data->is_admin == 1) {echo "Yes";}
                    else {echo "No";}
                echo "</td>";

                echo "<td class='edit_attr'>";
                    if ($user_data->is_admin == 1) {
                        echo "-";
                    }
                    else {
                        echo "<img src=\"../images/edit.jpg\" style=\"width:20px;height:20px;\">";
                        echo "<img src=\"../images/remove.jpg\" style=\"width:20px;height:20px;\">";
                    }
                echo "</td>";

            echo "</tr>";
        }
    ?>

<!-- if standard user is connected -->
<?php }  else { ?>

    <?php
        // get user's info
        $user_attributes = $credentials->getUserAttributes($_SESSION['user_id']);

        // print table

        echo "<table class='cred_table' style='width:600px;'>";
            echo "<tr>";
                echo "<th class='user_th'>User</th>";
                echo "<th>Attributes</th>";
                echo "<th>Admin</th>";
            echo "</tr>";

            echo "<tr>";
                echo "<td class='empty_line'></td>";
            echo "</tr>";

            echo "<tr>";

                echo "<td>";
                    echo "<div class=name>";
                        echo $_SESSION["user_name"];
                    echo "</div>";
                    echo "<div class=email>";
                        echo $_SESSION["user_email"];
                    echo "</div>";
                echo "</td>";

                echo "<td class=attr>";
                    echo "<ul>";
                    foreach ($user_attributes as $key => $value) {
                        //echo "$key => $value\n"; // debug
                        if ($key != "user_id") {
                            echo "<li>";
                                echo "$key = $value";
                            echo "</li>";
                        }
                    }
                    echo "</ul>";
                echo "</td>";

                echo "<td class='role'>";
                    echo "No";
                echo "</td>";

            echo "</tr>";
    ?>


<?php } ?>
                </table>

                <!-- Foot buttons -->
        		<table class="actions">
        			<tr>
        				<td class="button">
        				<span style="margin: 0 2%;"> 
        				<input 
                    		type="button" 
                    		value="<?php echo WORDING_LOGOUT; ?>" 
                    		class="small_button"
                    		onclick='location.href = "index.php?logout";' 
                		/> 
        				</span>
        				<span style="margin: 0 2%;"> 
        				<input 
                    		type="button" 
                    		value="<?php echo WORDING_EDIT_USER_DATA; ?>" 
                    		class="small_button"
                    		onclick='location.href = "edit.php";' 
                		/> 
        				</span>
        				</td>
        			</tr>
        		</table>

        		<?php include '../copyright.php';?>
        	</main>

    </body>

</html>

<?php include('_footer.php'); ?>
