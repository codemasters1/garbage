$(function() {
    var canvas_daily = document.getElementById("daily");
    canvas_daily.width = 600;
    canvas_daily.height = 300;
    var ctx_daily = canvas_daily.getContext("2d");

    var canvas_weekly = document.getElementById("weekly");
    canvas_weekly.width = 600;
    canvas_weekly.height = 300;
    var ctx_weekly = canvas_weekly.getContext("2d");


    var ws = new WebSocket("ws://server:8080/ws");
    ws.onopen = function()
    {
        ws.send("OK");
    };
    ws.onmessage = function(evt) 
    { 
        var d = JSON.parse(evt.data);

        console.log(d);

        var data_daily = {
            labels: [],
            datasets: [{
                fillColor: "rgba(220,220,220,0.2)",
                strokeColor: "rgba(100,100,100,1)",
                pointColor: "rgba(220,220,220,1)",
                pointStrokeColor: "#000",
                pointHighlightFill: "#000",
                pointHighlightStroke: "rgba(220,220,220,1)",
                data: []
            }]
        };

        var data_weekly = {
            labels: [],
            datasets: [{
                fillColor: "rgba(220,220,220,0.2)",
                strokeColor: "rgba(100,100,100,1)",
                pointColor: "rgba(220,220,220,1)",
                pointStrokeColor: "#000",
                pointHighlightFill: "#000",
                pointHighlightStroke: "rgba(220,220,220,1)",
                data: []
            }]
        };

        for(var i = 0; i < d.daily.hours.length; i++)
        {
            data_daily.labels.push(d.daily.hours[i] + ":00");
        }

        data_daily.datasets[0].data = d.daily.temps;
        console.log(data_daily);



        for(var i = 0; i < d.weekly.days.length; i++)
        {
            var item = d.weekly.days[i];
            var label = "";


            if (item[1] == 12)
            {
                label = item[0];
            }
            else if (item[1] % 12 == 0)
            {
                label = item[1] + ":00";
            }

            data_weekly.labels.push(label);
        }

        data_weekly.datasets[0].data = d.weekly.temps;
        console.log(data_weekly);

        var options = {
            bezierCurve: true,
            showTooltips: false,
            pointDot: false,
            animation: false
        };
        
        
        new Chart(ctx_daily).Line(data_daily, options);
        new Chart(ctx_weekly).Line(data_weekly, options);
    };
});