<?php
    //POST apikey + hex encoded tag + is in ESP DB
    //

    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'php_esp');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'roboclub_access_control');

    define('API_KEY', 'r2543fgo2');

    //variables posted from ESP, sync with ESP
    define('ADD_TAG', 'ADD_TAG');
    define('DEL_TAG', 'DEL_TAG');
    define('LOCK', 'LOCK');
    define('UNLOCK', 'UNLOCK');
    define('SCAN', 'SCAN');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    if ($mysql -> connect_error) {
        echo "Failed to connect to MySQL: " . $mysql -> connect_error;
        exit();
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = strtoupper($_POST["Tag"]);
        $action = ($_POST["Action"]);

        $query = "INSERT INTO entries_esp(tag_hex) VALUES(\"$tag_hex\")";

        switch($action) {
            case ADD_TAG:
                $query = "CALL tag_added_esp(\"$tag_hex\")";
                break;
            case DEL_TAG:
                $query = "CALL tag_deleted_esp(\"$tag_hex\")";
                break;
            case LOCK:
                $query = "INSERT INTO actions_esp(tag_hex, action) VALUES(\"$tag_hex\", \"LOCK\")";
        }

        if($mysql->query($query)) {
            echo "success";
        } else {
            echo "fail";
        }
    }

    $mysql->close();

?>