import sqlite3
conn = sqlite3.connect('notes.db')
c = conn.cursor()
c.execute("CREATE TABLE `messages` (`id` INTEGER PRIMARY KEY, `from` TEXT, `to` TEXT, `timestamp` INTEGER, `message` TEXT, `received` INTEGER)")
c.execute("INSERT INTO `messages` VALUES (NULL, 'mader', 'darnek', 0, 'test', 0)")
conn.commit()
conn.close()