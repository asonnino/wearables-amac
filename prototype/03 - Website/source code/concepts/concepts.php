<!DOCTYPE html>
<html>

<!-- HEAD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
<head>
	<meta charset="utf-8" />
	<link rel="stylesheet" href="/style/main.css"/>
	<link rel="stylesheet" href="/style/form.css"/>
	<script type="text/javascript" src="/functions.js"></script>
	<title>Concepts</title>
</head>


<!-- BODY - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -->
<body class="concepts_page"> 
<?php include '../menu/menu.php';?>

	<header>
		<h1 class="bottom_30">Concepts</h1>
	</header>

	<main>


		<section>
<p>This paper presents a flexible privacy enhancing system from its architecture to the prototyping level. The system takes advantage from anonymous credentials and is based on the protocols developed by <a href="/references/references.php">[12]</a>. Three main entities are involved in the system: a main server, wearable devices and localisation beacons. In this multi-purpose architecture, the server firstly issue some anonymous credentials to the wearables. Then, each time a wearable reach a particular physical location (gets close to a localisation beacon) where it desires to perform an action, it starts presenting its credentials in order to ask the server the execution of that a particular action. Both the design of the wearable and the server remain generic and scalable in order to encourage further enhancements and easy integration into real-world applications; i.e., the central server can manage an arbitrary number of devices, each device can posses an arbitrary number of credentials and the coverage area of the localisation system is arbitrarily extendable.</p>
		</section>
		<section>
			<h2>Motivations</h2>
<p><i>Why do we need security on wearable devices ?</i> The primary reason comes from the fact that, being in direct contact with the user, wearable devices have access to very private and sensitive user’s information more often than traditional technologies. The huge and increasing diversity of wearable technologies makes almost any kind of information at risk, going from medical records to personal habits and lifestyle. For that reason, when considering wearables, it is particularly important to introduce appropriate technologies to protect these data, and it is primary that both the user and the engineer are aware of the exact amount of collected information as well as the potential threats pending on the user’s privacy. Moreover, it has also to be considered that the privacy of the wearable’s user is not the only one at risk. In fact, more and more devices are not limited to record the user’s activity, but can also gather information about people standing around <a href="/references/references.php">[32, 22, 11]</a>.</p>
<p>For the above reasons, one of the most today’s expanding trends concerns the realisation of privacy enhancing technologies (PET), which allows to protect personally identifiable information and pre- serve the user’s privacy while benefitting of many advantages of the most modern technologies. In that purpose, constructions like anonymous credentials <a href="/references/references.php">[9, 10, 13, 14, 26]</a> have been developed. These technologies allow authenticated transactions between users and servers to remain anonymous, and for that reason, consist in powerful tools for the user’s privacy protection. Finally, the large diversity of today’s wearable devices includes applications going from automatic doors opening to as far as the field of health monitoring <a href="/references/references.php">[30]</a>. For that reason, it is particularly complicated to rely on a common standard architecture ensuring security and efficiency at the same time. It is therefore necessary to possess a set of reliable constructions in order to select the most appropriate depending on the application. Many new challenges appeared with the introduction of wearable and embedded technologies that are just waiting to be overcome.</p>
		</section>
		<section>
			<h2>System's Architecture</h2>
<p>The complete system’s architecture can be modelled as depicted in the figure below. Roughly speaking, the server starts by issuing some anonymous credentials granting a privilege level <i>j</i> to a wearable. Then, each time this wearable reaches some particular physical location (gets close to a localisation beacon) where it desires to perform an action requiring a privilege level of <i>j</i>, it starts presenting its credentials in order to ask the server the execution of that particular action. In fact, the attributes of the credentials represent the privilege level of the wearable’s user. More specifically, a privilege level <i>j</i> is determined by a set <i>&Phi;j</i>, such that if the user possesses an attributes <i>mj</i> included in <i>&Phi;j</i>, then he has a privilege level of <i>j</i>. Therefore, this user can ask the server to execute an action requiring such privileges.</p>

<div class="figure_right">
<img src="/images/complete_architecture.jpg" style="width:500px;">
</div>

<p>A web interface is used to manage and display the device’s functions. From that interface, each user and admin access the system. The admin’s role consists in granting credentials to the users and in managing the devices’ assignments, while users are only able to observe the credentials they own. The website is securely connected to the main server (e.g., using SSL) which possesses a database storing all the needed information; e.g., the list of users and their associated device, the device’s credentials, etc.</p> 
<p>During the setup phase, the server issues the credentials to a selected device. The credentials’ issuance is a short-range process. In fact, the wearable needs to be physically close to the server to allow the admins to physically verify, once and for all, the identity of the wearables’ users. In order to improve security and battery life, the wearable only communicates using extremely low-power and short-range radio waves (dotted line on the figure).</p>
<p>The server beacons can be seen as continuities of the main server and have essentially two roles: the first is to operate as an interface between the wearables and the server, and the second is to act as the RF localisation system. When approaching these beacons, the wearable receives a location’s update. To that purpose, it starts an unilateral authentication mechanism defined by the ISO 9798-3 standard <a href="/references/references.php">[37]</a> where only the beacon is authenticated. This process allows the wearable to accepts only authenticated location’s code and to negotiate a symmetric key with the beacon without compromising its anonymity. At that point, if the user possesses some credentials allowing him to perform a particular action at that location, the device automatically presents these credentials in order to anonymously prove to the server that it has the required privilege level to perform the desired action. The wearable does not talk directly to the server but uses the beacon as interface instead. Then, it starts the credentials’ showing protocol by sending all the needed information encrypted under the derived shared key (for instance, using classic block ciphers like AES in CBC mode <a href="/references/references.php">[17]</a>). The beacon receives these information and decrypts them. Since both the server and the beacon are under the admins’ control, they can easily share a symmetric key. The beacon uses this key to pass the received credentials’ data directly to the server using an Authenticated Encryption with Associated Data (AEAD) scheme, like GCM-AES <a href="/references/references.php">[27]</a>. In fact, these AEAD schemes can be implemented extremely efficiently on industrial hardware <a href="/references/references.php">[25]</a>. This communication does not need to be performed on radio frequencies. Moreover, this beacon-server symmetric key can also be useful for completely different purposes like a beacon’s code updates.
<p>Finally, once the credentials have been successfully verified by the server, the server issues a signed request to an external entity (which can be, for instance, an automatic door, an alarm system or any compatible IoT entity) to perform the requested action. This last interaction between the server and the external entity is signed along with a fresh random nonce with the server’s public key.</p>


<p>A concrete example of utilisation of this systems could be associating the external entity to an automatic office door. Therefore, each employee working on that office possesses a wearable with the credentials to access it. Subsequently, when an authorised employee steps close to the door, the device automatically detects its position, knows it is close to the door, and correctly presents the credentials to the server. Then, after credentials’ verification, the server opens the door. Note the usability advantage of having such common operations executed automatically and based on the location; i.e, the user does not even need to present a smart card since the wearable reacts by its own.</p>
		</section>
		<section style="clear:left;">
			<h2>More Information</h2>
<p class="bottom_30">More information as well as details on a prototype's implementation and PCB layouts can be found in the original paper freely downloadable here under.</p>
<div style="text-align:center;">		
<a class="pdf_button" href="/paper.pdf" download="Main.pdf">Download Full PDF</a>
</div>
		</section>
	</main>

<div style="clear:left;"><?php include '../copyright.php';?></div>
</body>

</html>
