<?php

include 'config.php';

// $_GET
// $_POST

if ($auth_key != $_POST['auth_key']) {
    exit();
}

$query = "SELECT `height_percentage` FROM `height_history` ORDER BY `height_datetime` DESC LIMIT 1";

$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

if (isset($row['height_percentage'])) {
    echo $row['height_percentage'];
} else {
    echo 0;
}

$result->close();
$mysqli->close();
