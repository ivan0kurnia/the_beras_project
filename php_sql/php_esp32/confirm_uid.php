<?php

include 'config.php';

// $_GET
// $_POST

if ($auth_key != $_POST['auth_key']) {
    exit();
}

$card_uid = $_POST['card_uid'];

$query =
    "SELECT `user_firstname`, 
            `user_lastname`, 
            `user_staple_quota`, 
            `tap_datetime` AS 'last_tap_datetime', 
            NOW()          AS 'current_datetime' 
    FROM    `beras_users` 
            LEFT JOIN `tap_history` 
                   ON `tap_history`.`card_uid` = `beras_users`.`card_uid` 
    WHERE   `beras_users`.`card_uid` = '$card_uid' 
    ORDER   BY `tap_datetime` DESC 
    LIMIT   1";

$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

// If nothing queried from the database, the user definitely doesn't exist
// if (empty($row)) {
//     // U: UNAUTHORIZED
//     echo 'U' . '_' . 'USER DOES NOT EXIST';
    
//     $query = "INSERT INTO `unknown_taps` (`card_uid`) VALUES ('$card_uid')";
//     $result = $mysqli->query($query);

//     // $result->close(); // Leads to HTTP error 500
//     $mysqli->close();
//     exit();
// }

// When a new user is created, its 'last_tap_datetime' remains empty until its first tap
if (isset($row['last_tap_datetime'])) {
    $last_tap_datetime = new DateTime($row['last_tap_datetime']);
    $current_datetime = new DateTime($row['current_datetime']);

    // Change time zones only for calculation
    $last_tap_datetime->add(new DateInterval($device_timezone));
    $current_datetime->add(new DateInterval($device_timezone));

    $last_tap_week_number = idate('W', $last_tap_datetime->getTimestamp());
    $current_week_number = idate('W', $current_datetime->getTimestamp());

    if ($last_tap_week_number === $current_week_number) {
        $date_difference = $current_datetime->diff($last_tap_datetime);

        // Two similar week numbers is a possibility in the same year, this checks for that
        if ($date_difference->days < 7) {
            // U: UNAUTHORIZED
            echo 'U' . '_' . 'STAPLE ALREADY TAKEN THIS WEEK';

            $result->close();
            $mysqli->close();
            exit();
        }
    }
}

// 'current_datetime_string' is left in GMT for querying back into the database
$user_firstname = $row['user_firstname'];
$user_lastname = $row['user_lastname'];
$user_staple_quota = $row['user_staple_quota'];
$current_datetime_string = $row['current_datetime'];

$query = "INSERT INTO `tap_history` (`card_uid`, `tap_datetime`) VALUES ('$card_uid', '$current_datetime_string')";
$result = $mysqli->query($query);

// A: AUTHORIZED
echo 'A' . '_' . $user_firstname . ' ';
echo $user_lastname . '_' . $user_staple_quota;

// $result->close(); // Leads to HTTP error 500
$mysqli->close();