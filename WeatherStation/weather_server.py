from __future__ import print_function
from sqlalchemy import create_engine, MetaData, select
from tornado import websocket, web, ioloop
import json


engine = create_engine('mysql://xx:xx@localhost/weather_station', echo=True)
meta = MetaData(engine)
connection = engine.connect()

def get_daily_temps():
    result = connection.execute("""
        SELECT SQL_NO_CACHE HOUR(t), temp FROM 
            (SELECT
                time AS t,
                ROUND(AVG(temperature), 2) AS temp
            FROM
                temps
            WHERE
                time >= NOW() - INTERVAL 1 DAY
            GROUP BY
                HOUR(time) 
            ORDER BY
                time DESC
            LIMIT 24) sub
        ORDER BY t ASC
        """)

    hours = []
    temps = []

    for h, t in result.fetchall():
        hours.append(h)
        temps.append(t)

    return dict(hours=hours, temps=temps)

def get_weekly_temps():
    result = connection.execute("""
        SELECT SQL_NO_CACHE DAYNAME(t), HOUR(t), temp FROM 
            (SELECT
                time AS t,
                ROUND(AVG(temperature), 2) AS temp
            FROM
                temps
            WHERE
                time >= NOW() - INTERVAL 7 DAY
            GROUP BY
                DAY(time), HOUR(time) DIV 2
            ORDER BY
                time DESC
            LIMIT 98) sub
        ORDER BY t ASC
        """)

    days = []
    temps = []

    for d, h, t in result.fetchall():
        days.append((d[:3], h))
        temps.append(t)

    return dict(days=days, temps=temps)

class IndexHandler(web.RequestHandler):
    def get(self):
        self.render("index.html")

class SocketHandler(websocket.WebSocketHandler):
    def open(self):
        pass

    def on_message(self, msg):
        data = dict(daily=get_daily_temps(), weekly=get_weekly_temps())
        self.write_message(json.dumps(data))

    def on_close(self):
        pass

app = web.Application([
    (r'/', IndexHandler),
    (r'/ws', SocketHandler),
    (r'/static/(.*)', web.StaticFileHandler, {'path': '.'}),
])

if __name__ == '__main__':
    app.listen(8080)
    ioloop.IOLoop.instance().start()