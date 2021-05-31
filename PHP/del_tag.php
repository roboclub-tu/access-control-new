<?php

    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'php_esp');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'roboclub_access_control');

    define('API_KEY', 'r2543fgo2');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    if ($mysql -> connect_error) {
        echo "Failed to connect to MySQL: " . $mysql -> connect_error;
        exit();
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = $_POST["Tag"];
        
        $sql = "UPDATE `roboclub_access_control`.`users`
        SET in_esp = 0
        WHERE tag_hex = \"$tag_hex\"";

        if($mysql->query($sql)) {
            echo("deleted sucessfully");
        } else {
            echo("mysql error");
        }
    }
    
    $mysql->close();

?>