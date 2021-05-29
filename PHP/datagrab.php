<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <table>
        <tr>
            <th>Name</th>
            <th>In Esp</th>
            <th>Timestamp</th>
        </tr>
        <?php
            //database connection
            define('DB_HOST', 'localhost');
            define('DB_USENAME', 'roboclub_access_esp');
            define('DB_PASSWORD', 'test123');
            define('DB_NAME', 'roboclub_access_control');

            $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

            if(!$mysql) {
                die("Connection failed: " . $mysql->error);
            }

            //last_entries is a view that shows last 200 entries
            $query = "SELECT * FROM last_entries";

            $result = $mysql->query($query);

            if($result-> num_rows > 0) {
                while($row = $result->fetch_assoc()) { //fetch an array within an array
                    //print 1 line
                    echo "<tr><td>" . $row["name"] . "</td><td>" . $row["in_esp"] . "</td><td>" . $row["timestamp"] . "</td></tr>";
                }
                echo "</table>";
            } else {
                echo "No Result </table>";
            }

            

            //free memory
            $result->close();

            $mysql->close();

            
        ?>
    </table>
</body>
</html>