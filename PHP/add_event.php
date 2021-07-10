<?php
    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'root');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'roboclub_access_control');

    //TODO change to a config file or a json object
    define('API_KEY', 'r2543fgo2');

    //variables posted from ESP, sync with ESP
    //TODO check if you can get them from a .h file to automatically sync with ESP
    define('ADD_TAG', 'ADD_TAG');
    define('DEL_TAG', 'DEL_TAG');
    define('LOCK', 'LOCK');
    define('UNLOCK', 'UNLOCK');
    define('SCAN', 'SCAN');
    define('ERROR', 'ERROR');

    //SQL statements used for different events
    define('ADD_TAG_QUERY', 'CALL tag_added_esp(\"$tag_hex\")');
    define('DEL_TAG_QUERY', 'CALL tag_deleted_esp(\"$tag_hex\")');
    define('LOCK_QUERY', 'INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"LOCK\")');
    define('UNLOCK_QUERY', 'INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"UNLOCK\")');
    define('SCAN_QUERY', 'INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"SCAN\")');
    define('ERROR_QUERY', 'INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"ERROR\")');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    if ($mysql -> connect_error) {
        echo "Failed to connect to MySQL: " . $mysql -> connect_error;
        exit();
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = strtoupper($_POST["Tag"]);
        $event = ($_POST["Event"]);

        $query = "";

        switch($action) {
            case ADD_TAG:
                $query = ADD_TAG_QUERY;
                break;
            case DEL_TAG:
                $query = DEL_TAG_QUERY;
                break;
            case LOCK:
                $query = LOCK_QUERY;
                break;
            case UNLOCK:
                $query = UNLOCK_QUERY;
                break;
            case SCAN:
                $query = SCAN_QUERY;
                break;
        }

        if($mysql->query($query)) {
            echo "success";
        } else {
            echo "fail";
        }
    }

    $mysql->close();

?>