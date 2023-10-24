
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Servidor WEB NodeMCU</title>

        <!-- bootstrap -->
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" />
        <!-- chart library -->
        <script src="https://code.highcharts.com/highcharts.js"></script>

        <style>
            footer {
                font-size: x-small;
            }

            img {
                height: auto;
                border: 4px solid white;
            }
        </style>

        <link rel="icon" href="data:,">
    </head>
    <body class="d-flex flex-column min-vh-100">
    

    <!-- Contenido funcional de la página -->
    <div class="container">
        
        <div class="card">
            <h2>Graficos desde ADC</h2>
            

            <div id="chart-ADC" class="container"></div>
        </div>
        <form action="/get">
      Email Address: <input type="email" name="email_input" value="%EMAIL_INPUT%" required><br>
      Enable Email Notification: <input type="checkbox" name="enable_email_input" value="true" %ENABLE_EMAIL%><br>
      Temperature Threshold: <input type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required><br>
      <input type="submit" value="Submit">
    </form>
    </div>
    
  </body>
  <script>
    function updateTextInput(val) {
        console.log(val);
        document.getElementById('pwmInput').value = val;
        document.getElementById('textInput').value = val;
    }

    var chartADC = new Highcharts.Chart({
        chart: { renderTo: 'chart-ADC' },
        title: { text: 'ADC' },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            },
            series: { color: '#18009c' }
        },
        xAxis: {
            title: { text: 'Tiempo (s)' }
        },
        yAxis: {
            title: { text: 'Temperatura en °C' }
        },
        credits: { enabled: false }
    });
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                console.log(this.responseText);
                chartADC.series[0].setData([]);
                data = this.responseText.split(",");
                for (var i = 0; i < data.length; i++) {
                    var x = i,
                        y = parseFloat(data[i]);
                    chartADC.series[0].addPoint([x, y], true, false, true);
                }
            }
        };
        xhttp.open("GET", "/chart", true);
        xhttp.send();
    }, 10000);
    

</script></html>
)rawliteral";