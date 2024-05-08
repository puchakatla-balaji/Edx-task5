const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Weather Station</title>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
  <script>
    $(document).ready(function() {
      setInterval(function() {
        $.ajax({
          url: "/readADC",
          success: function(data) {
            var jsonData = JSON.parse(data);
            $("#rain").html("Rain: " + jsonData.Rain);
            $("#temperature").html("Temperature: " + jsonData.Temperature);
            $("#humidity").html("Humidity: " + jsonData.Humidity);
          }
        });
      }, 1000);
    });
  </script>
</head>
<body>
  <h1>ESP8266 Weather Station</h1>
  <div id="rain">Rain: </div>
  <div id="temperature">Temperature: </div>
  <div id="humidity">Humidity: </div>
</body>
</html>
)=====";
