<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<title>Last Entries</title>
<style type = "text/css">
    table {
        border-collapse: collapse;
        width: 100%;
        color: #588c7e;
        font-family: monospace;
        font-size: 25px;
        text-align: left;
    }
    th {
        background-color: #588c7e;
        color: white;
    }
    tr:nth-child(even) {
        background-color: #f2f2f2;
    }
</style>
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
            define('DB_USENAME', 'php_esp');
            define('DB_PASSWORD', 'test123');
            define('DB_NAME', 'roboclub_access_control');

            $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

            if ($mysql -> connect_error) {
                echo "Failed to connect to MySQL: " . $mysql -> connect_error;
                exit();
            }

            //last_entries is a view that shows last 200 entries
            $sql = "SELECT * FROM last_entries";

            $result = $mysql->query($sql);

            if($result->num_rows > 0) {
                while($row = $result->fetch_assoc()) { //fetch an array within an array
                    //print 1 line
                    echo "<tr><td>" . $row["name"] . "</td><td>" . $row["in_esp"] . "</td><td>" . $row["timestamp"] . "</td></tr>";
                }
                echo "</table>";
            } else {
                echo "No Result";
            }

            $mysql->close();

            
        ?>
    </table>
</body>
</html>