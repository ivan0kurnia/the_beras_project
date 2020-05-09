<?php

include 'config.php';

$query = "SELECT Concat_ws(' ', `user_firstname`, `user_lastname`) AS 'name', 
       `tap_history`.`card_uid`, 
       `tap_datetime` 
FROM   `tap_history` 
       JOIN `beras_users` 
         ON `tap_history`.`card_uid` = `beras_users`.`card_uid` 
ORDER  BY `tap_datetime` DESC ";

$result = $mysqli->query($query);

$table = array();
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    array_push($table, $row);
}

for ($index = 0; $index < count($table); ++$index) {
    $tap_datetime_string = $table[$index]['tap_datetime'];
    
    $tap_datetime = new DateTime($tap_datetime_string);
    $tap_datetime->add(new DateInterval($device_timezone));
    
    $table[$index]['tap_datetime'] = $tap_datetime->format('Y-m-d H:i:s');
}

echo json_encode($table);

$result->close();
$mysqli->close();
