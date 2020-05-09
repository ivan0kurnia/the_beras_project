<?php

$device_timezone = 'PT07H';

$db_host = 'mysql.hostinger.in';
$db_user = 'u216319883_rwv_m';
$db_pass = 'atmberas';
$db_name = 'u216319883_beras_m_db';

$mysqli = new mysqli($db_host, $db_user, $db_pass, $db_name);

if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') ' . $mysqli->connect_error);
}
