<?php

include 'config.php';

$table_name = 'beras_users';
$query = "
    SELECT card_uid, 
       IF(Isnull(user_middlename), Concat_ws(' ', user_firstname,  user_middlename), 
       Concat_ws(' ', user_firstname, user_middlename, user_lastname)) AS 
       'user_name', 
       user_gender, 
       Concat_ws(' | ', Ifnull(user_landline_number, '-'), 
       Ifnull(user_cellphone_number, '-'))                               AS 
       'phone_number', 
       user_address_ktp, 
       user_address_current, 
       user_religion, 
       user_marital_status, 
       user_occupation, 
       Concat_ws(', ', user_birthplace, user_birthdate) AS 'user_birth', 
       user_liability, 
       user_staple_quota, 
       user_created_by, 
       user_creation_date 
FROM   `beras_users` 
ORDER  BY user_id ";

$result = $mysqli->query($query);

$table = array();
while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
    array_push($table, $row);
}

for ($index = 0; $index < count($table); ++$index) {
    $tap_datetime_string = $table[$index]['user_creation_date'];
    
    $tap_datetime = new DateTime($tap_datetime_string);
    $tap_datetime->add(new DateInterval($device_timezone));
    
    $table[$index]['user_creation_date'] = $tap_datetime->format('Y-m-d H:i:s');
}

echo json_encode($table);

$result->close();
$mysqli->close();
