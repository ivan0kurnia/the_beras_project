<?php

include 'config.php';

// $_GET
// $_POST

$admin_username = $_POST['admin_username'];
$admin_password = $_POST['admin_password'];
$admin_firstname = $_POST['admin_firstname'];
$admin_lastname = $_POST['admin_lastname'];
$admin_email  = $_POST['admin_email'];
$admin_phone_number = $_POST['admin_phone_number'];

$query = "SELECT * FROM beras_admins WHERE admin_email='$admin_email'";
$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

if (isset($row)) {
    echo 'Email already exists. Please use another email address.';
} else {
    $query = "INSERT INTO `beras_admins` (`admin_username`,`admin_password`,`admin_firstname`,`admin_lastname`, `admin_email`, `admin_phone_number`) VALUES ('$admin_username', '$admin_password', '$admin_firstname', '$admin_lastname', '$admin_email', '$admin_phone_number' )";

    if ($mysqli->query($query)) {
        echo 'User registration successful.';
    } else {
        echo 'Registering admin to the database failed.';
    }
}

$mysqli->close();
