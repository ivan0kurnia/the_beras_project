<?php

include 'config.php';

$query = "SELECT 
       `height_percentage` 
FROM   `height_history` 
ORDER  BY `height_datetime` DESC LIMIT 1";

$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

echo json_encode($row);

$result->close();
$mysqli->close();
