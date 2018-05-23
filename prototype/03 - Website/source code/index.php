<?php
// start session
session_start();
// set current name
$_SESSION['current_page'] = "index.php";
?>


<!DOCTYPE html>
<html>

<!-- HEAD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
<head>
	<meta charset="utf-8" />
	<link rel="/images/icon" href="/icon.png">
	<link rel="stylesheet" href="/style/main.css"/>
	<script type="text/javascript" src="http:///embed/S9CtmpuIdwPCNtfo"></script>
	<title>Home</title>
</head>


<!-- BODY - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
<body class="home_page"> 
<?php include 'menu/menu.php';?>

	<main>
		<?php 
			echo '<script type="text/javascript">'
				, 'location.href = "/php-login-advanced-master/index.php"; '
				, '</script>';
		?>
	</main>

	<div><?php include 'copyright.php';?></div>
</body>

</html>
