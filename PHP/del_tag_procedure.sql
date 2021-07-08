delimiter ||
CREATE procedure tag_deleted_esp (IN tag VARCHAR(10))
BEGIN
UPDATE users
        SET in_esp = 0
        WHERE tag_hex = tag;
INSERT INTO actions_esp (tag_hex, action) VALUES(tag, 'deleteTag');
end;
||
delimiter ;