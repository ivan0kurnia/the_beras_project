<?php

include 'config.php';

$query = "SELECT CONCAT_WS(' ', `admin_firstname`, `admin_lastname`) AS 'admin_name', `admin_email`, `admin_phone_number` FROM `beras_admins` ORDER BY `admin_id`";
$result = $mysqli->query($query);

$table = array();
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    array_push($table, $row);
}

echo json_encode($table);

$result->close();
$mysqli->close();
