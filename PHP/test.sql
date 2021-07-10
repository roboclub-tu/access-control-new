call tag_added_esp('F32OJ2F');
call tag_deleted_esp('F32OJ2F');
call tag_added_esp('F32OJ2F');
INSERT INTO events_esp(tag_hex, event) VALUES("F32OJ2F", "SCAN");
INSERT INTO events_esp(tag_hex, event) VALUES("F32OJ2F", "ERROR");
INSERT INTO events_esp(tag_hex, event) VALUES("F32OJ2F", "LOCK");
INSERT INTO events_esp(tag_hex, event) VALUES("F32OJ2F", "UNLOCK");

select * from last_events;