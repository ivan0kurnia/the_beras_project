<?php

include 'config.php';

// $_GET
// $_POST

if ($auth_key != $_POST['auth_key']) {
    exit();
}

$current_height_percentage = $_POST['height_percentage'];

$allowed_percentages = ['0', '15', '20', '40', '60', '80'];

$current_height_is_allowed = false; // Not yet allowed, initially
for ($percentage = 0; $percentage < count($allowed_percentages); ++$percentage) {
    if ($current_height_percentage === $allowed_percentages[$percentage]) {
        $current_height_is_allowed = true; // Now it is allowed when one of them is identical
        break;
    }
}

if (!$current_height_is_allowed) {
    // N: NOT UPDATED
    echo 'N' . '_' . 'THE SUBMITTED HEIGHT PERCENTAGE IS NOT ALLOWED';

    $result->close();
    $mysqli->close();
    exit();
}

$query = "SELECT `height_percentage` FROM `height_history` ORDER BY `height_datetime` DESC LIMIT 1";

$result = $mysqli->query($query);
$row = $result->fetch_array(MYSQLI_ASSOC);

$last_height_percentage = $row['height_percentage'];

if ($last_height_percentage === $current_height_percentage) {
    // N: NOT UPDATED
    echo 'N' . '_' . 'CURRENT HEIGHT PERCENTAGE IS THE SAME AS THE LAST ONE';

    $result->close();
    $mysqli->close();
    exit();
}

$query = "INSERT INTO `height_history` (`height_percentage`) VALUES ('$current_height_percentage')";
$result = $mysqli->query($query);

// U: UPDATED
echo 'U' . '_' . $current_height_percentage;

// $result->close(); // Leads to HTTP error 500
$mysqli->close();