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

    if (true) {

        //TODO improve against injection
        $tag_hex = strtoupper($_POST["Tag"]);
        $event = ($_POST["Event"]);

        $query = "";

        switch($event) {
            case ADD_TAG:
                $query = "CALL tag_added_esp(\"$tag_hex\")";
                break;
            case DEL_TAG:
                $query = "CALL tag_deleted_esp(\"$tag_hex\")";
                break;
            case LOCK:
                $query = "INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"LOCK\")";
                break;
            case UNLOCK:
                $query = "INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"UNLOCK\")";
                break;
            case SCAN:
                $query = "INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"SCAN\")";
                break;
            default:
                $query = "INSERT INTO events_esp(tag_hex, event) VALUES(\"$tag_hex\", \"ERROR\")";
                break;
        }
        echo $query;

        if($mysql->query($query)) {
            echo "success";
        } else {
            echo "fail";
        }
    }

    $mysql->close();

?>