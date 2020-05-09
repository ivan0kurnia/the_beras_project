<?php

    include 'config.php';
    
    // $_GET
    // $_POST
    
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    $table_name = 'beras_admins';
    $query = "SELECT admin_username, admin_firstname, admin_lastname, admin_email, admin_phone_number FROM `$db_name`.`$table_name` WHERE admin_username = '$username' AND admin_password = '$password' LIMIT 1";
    
    $result = $mysqli->query($query);
    $row = $result->fetch_array(MYSQLI_ASSOC);
    
    if (empty($row))
    {
        echo 'NO_MATCHING_USER';
        
        $result->close();
        $mysqli->close();
        exit();
    }
    
    $query =
    "SELECT     `height_percentage` 
    FROM        `height_history` 
    ORDER  BY   `height_datetime` DESC LIMIT 1";
    
    $result = $mysqli->query($query);
    $height = $result->fetch_array(MYSQLI_NUM);
    
    $row['height_percentage'] = $height[0];
    
    echo json_encode($row);
    
    $result->close();
    $mysqli->close();