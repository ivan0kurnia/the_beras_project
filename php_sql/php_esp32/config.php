<?php

// Change to 'PT08H' or 'PT09H' for WITA or WIT time zone.
$device_timezone = 'PT07H';

$auth_key = '64bf9ec7e43d43c544c059380d171162';

$db_host = 'mysql.hostinger.in';
$db_user = 'u216319883_rwv_m';
$db_pass = 'atmberas';
$db_name = 'u216319883_beras_m_db';

$mysqli = new mysqli($db_host, $db_user, $db_pass, $db_name);

if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') ' . $mysqli->connect_error);
}