<?php
include("config.php");
if (isset($_GET["carduid"]) && $_GET["carduid"] != "") {
    $sql = "SELECT * FROM data_sk WHERE id='" . $_GET["carduid"] . "'";
    $result = $con->query($sql);

    if ($result->num_rows == 1) {
        while ($row = $result->fetch_assoc()) {
            http_response_code(200);
            $curDateTime = new DateTime();
            $lastDateTime = new DateTime($row["last_tap"]);
            if ($curDateTime >= $lastDateTime->modify('+7 day')) {
                // Update last_tap on database
                $sql2 = "UPDATE data_sk SET last_tap='" . $curDateTime->format("Y-m-d H:i:s") . "' WHERE id='" . $_GET["carduid"] . "'";
                if ($con->query($sql2) === TRUE) {
                    echo "1" . ";" . $row["nama"] . ";" . $row["Berat"];
                } else {
                    echo "DENIED";
                }
            } else {
                echo "2" . "Belum 7 hari";
            }
        }
    } else {
        http_response_code(200);
        echo "0 result";
    }
    $con->close();
} else {
    http_response_code(404);
}
