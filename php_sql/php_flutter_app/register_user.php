<?php

include 'config.php';

// $_GET
// $_POST

$card_uid = $_POST['card_uid'];
$user_ktp = $_POST['user_ktp'];
$user_firstname = $_POST['user_firstname'];
$user_lastname = $_POST['user_lastname'];
$user_gender = $_POST['user_gender'];
$user_address_current = $_POST['user_address_current'];
$user_landline_number = $_POST['user_landline_number'];
$user_cellphone_number = $_POST['user_cellphone_number'];
$user_liability = $_POST['user_liability'];

$one_person_quota = 1750;
$user_staple_quota = $one_person_quota * $user_liability;
// $user_staple_quota = $_POST['user_staple_quota'];

$query = "SELECT * FROM beras_users WHERE card_uid='$card_uid'";
$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

if (isset($row)) {
    echo 'Email already exists. Please use another email address.';
} else {
    $query = "
INSERT INTO `beras_users` 
    (`card_uid`, 
     `user_ktp`, 
     `user_firstname`, 
     `user_lastname`, 
     `user_gender`, 
     `user_address_current`, 
     `user_landline_number`, 
     `user_cellphone_number`, 
     `user_liability`, 
     `user_staple_quota`) 
VALUES      ('$card_uid', 
     '$user_ktp', 
     '$user_firstname', 
     '$user_lastname', 
     '$user_gender', 
     '$user_address_current', 
     '$user_landline_number', 
     '$user_cellphone_number', 
     '$user_liability', 
     '$user_staple_quota')";

    if ($mysqli->query($query)) {
        echo 'User registration successful.';
    } else {
        echo 'Registering user to the database failed.';
    }
}

$mysqli->close();
