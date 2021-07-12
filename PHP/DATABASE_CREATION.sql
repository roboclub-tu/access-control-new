DROP TABLE IF EXISTS events_esp;
CREATE TABLE `roboclub_access_control`.`events_esp` (
  `_id` INT NOT NULL AUTO_INCREMENT,
  `tag_hex` VARCHAR(45) NULL,
  `event` ENUM('ADD_TAG', 'DEL_TAG', 'LOCK', 'UNLOCK', 'SCAN', 'ERROR') NULL DEFAULT 'ERROR',
  `timestamp` TIMESTAMP NULL DEFAULT NOW(),
  PRIMARY KEY (`_id`));

DROP TABLE IF EXISTS users;
CREATE TABLE `roboclub_access_control`.`users` (
  `tag_hex` VARCHAR(10) NOT NULL,
  `user_name` VARCHAR(45) NULL,
  `in_esp` TINYINT NULL DEFAULT 0,
  PRIMARY KEY (`tag_hex`));


DROP VIEW IF EXISTS last_events;
CREATE VIEW `last_events` AS
SELECT IFNULL(users.user_name, 'N/A') AS name, IFNULL(users.in_esp, '0') AS in_esp, IFNULL(events_esp.event, 'N/A') as event, events_esp.timestamp
FROM events_esp
LEFT JOIN users
ON users.tag_hex = events_esp.tag_hex
LIMIT 200;

DROP PROCEDURE IF EXISTS tag_deleted_esp;
delimiter ||
CREATE procedure tag_deleted_esp (IN tag VARCHAR(10))
BEGIN
UPDATE users
        SET in_esp = 0
        WHERE tag_hex = tag;
INSERT INTO events_esp (tag_hex, event) VALUES(tag, 'DEL_TAG');
END;
||
delimiter ;

DROP PROCEDURE IF EXISTS tag_added_esp;
delimiter ||
CREATE procedure tag_added_esp (IN tag VARCHAR(10))
BEGIN
INSERT INTO users(tag_hex, user_name, in_esp) VALUES (tag, tag, true)
ON DUPLICATE KEY UPDATE
in_esp = true;
INSERT INTO events_esp (tag_hex, event) VALUES(tag, 'ADD_TAG');
END;
||
delimiter ;
