<?php
  $num = $_GET["num"];
  $msg = $_GET["msg"];
  $api = "5e04299d9696d6e3e0e8e7ae8c99cbf8369a6e07";

  $url = 'https://smsany.herokuapp.com/api/sms/sendsms';
	$data = array('phoneNumber' => $num, 'message' => $msg, 'accessKey' => $api);

	$ch = curl_init($url);

	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, 'phoneNumber='.$num.'&message='.$msg.'&accessKey='.$api);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

	$response = curl_exec($ch);
	echo $response;
	curl_close($ch);
?>