<?php

    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'roboclub_access_esp');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'roboclub_access_control');

    define('API_KEY', 'r2543fgo2');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    if(!$mysql) {
        die("Connection failed: " . $mysql->error);
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = $_POST["Tag"];
        
        $sql_statement = "UPDATE `roboclub_access_control`.`users`
        SET in_esp = 0
        WHERE tag_hex = $tag_hex";

        if($mysql->query($sql_statement)) {
            echo("deleted sucessfully");
        } else {
            echo("mysql error");
        }
    }
    
    $mysql->close();

?>