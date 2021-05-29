<?php
    //POST apikey + hex encoded tag + is in ESP DB
    //

    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'roboclub_access_esp');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'access_control');

    define('API_KEY', 'r2543fgo2');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    if(!$mysql) {
        die("Connection failed: " . $mysql->error);
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = $_POST["Tag"];
    }
    

    $mysql->close();

?>