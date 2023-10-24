const char mail_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
<html>
<head>
    <title>ESP32 Temperature Alert</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Open+Sans">
    
    
    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    
    <style>
      
        body {
            font-family: 'Open Sans', sans-serif;
            text-align: center;
            background-color: #f4f4f4;
        }
        
        h2 {
            font-size: 2rem;
            margin-top: 20px;
            color: #333;
        }
        
        h3 {
            font-size: 1.5rem;
            background-color: #FF0000;
            color: white;
            padding: 10px;
            border-radius: 5px;
        }

        form {
            margin-top: 20px;
            background-color: white;
            padding: 20px;
            border-radius: 10px;
        }
        
        
        body {
            margin-bottom: 40px;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="row">
            <div class="col">
                <h2>DS18B20 Current Temperature Reading</h2>
                <h3>%TEMPERATURE% &deg;C</h3>
            </div>
        </div>
        <div class="row">
            <div class="col">
                <h2>Email Alert Notifications</h2>
                <form action="/get">
                    <div class="form-group">
                        <label for="email_input">Email Address</label>
                        <input type="email" class="form-control" id="email_input" name="email_input" value="%EMAIL_INPUT%" required>
                    </div>
                    <div class="form-check">
                        <input type="checkbox" class="form-check-input" id="enable_email_input" name="enable_email_input" value="true" %ENABLE_EMAIL%">
                        <label class="form-check-label" for="enable_email_input">Enable Email Notification</label>
                    </div>
                    <div class="form-group">
                        <label for="threshold_input">Temperature Threshold</label>
                        <input type="number" step="0.1" class="form-control" id="threshold_input" name="threshold_input" value="%THRESHOLD%" required>
                    </div>
                    <button type="submit" class="btn btn-primary">Submit</button>
                </form>
            </div>
        </div>
    </div>

    
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.11.6/dist/umd/popper.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
)rawliteral";

