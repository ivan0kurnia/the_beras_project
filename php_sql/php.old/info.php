<center>
    <h2>Data Tapping </h2>
    <table border="1">
        <tr>
            <th>NO</th>
            <th>nama</th>
            <th>Berat</th>
            <th>last_tap</th>
            <?php
            include 'config.php';
            // require_once('config.php');
            $tapping_info = mysqli_query($con, "SELECT * FROM data_sk");
            // echo $tapping_info;
            // var_dump($tapping_info);
            $no = 1;

            foreach ($tapping_info as $row) {
                echo "<tr>
            <td>$no</td>
            <td>" . $row['nama'] . "</td>
            <td>" . $row['Berat'] . "</td>
            <td>" . $row['last_tap'] . "</td>
            
              </tr>";
                $no++;
            }
            ?>
    </table>