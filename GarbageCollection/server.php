<?php
  $status = $_GET["status"];
  

  $url = "https://us-central1-project-7009880330546042272.cloudfunctions.net/intruderFunction/v1/intruder/$status";
	

	$ch = curl_init($url);

	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, 'phoneNumber='.$num.'&message='.$msg.'&accessKey='.$api);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

	$response = curl_exec($ch);
	echo $response;
	curl_close($ch);
