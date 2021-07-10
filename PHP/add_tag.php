<?php
    //POST apikey + hex encoded tag + is in ESP DB
    //

    //database connection
    define('DB_HOST', 'localhost');
    define('DB_USENAME', 'php_esp');
    define('DB_PASSWORD', 'test123');
    define('DB_NAME', 'roboclub_access_control');

    define('API_KEY', 'r2543fgo2');

    $mysql = new mysqli(DB_HOST, DB_USENAME, DB_PASSWORD, DB_NAME);

    //TODO rewrite mysql test
    if ($mysql -> connect_error) {
        echo "Failed to connect to MySQL: " . $mysql -> connect_error;
        exit();
    }

    if ($_POST["ApiKey"] == API_KEY) {

        //TODO improve against injection
        $tag_hex = strtoupper($_POST["Tag"]);

        //TODO procedure to test if tag already in db
        //   if yes, update
        //   if no, insert
        
        $sql_statement = "CALL tag_in_esp(\"$tag_hex\")";

        if($mysql->query($sql_statement)) {
            echo("inserted sucessfully");
        } else {
            echo("already in db / mysql error");
        }
    }

    $mysql->close();

?>