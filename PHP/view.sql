CREATE VIEW `last_actions` AS
SELECT IFNULL(users.name, 'N/A') AS name, IFNULL(users.in_esp, '0') AS in_esp, IFNULL(actions_esp.action, 'N/A'), actions_esp.timestamp
FROM actions_esp
LEFT JOIN users
ON users.tag_hex = actions_esp.tag_hex
LIMIT 200;