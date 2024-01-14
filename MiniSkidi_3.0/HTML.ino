// This file contains the HTML data for the MiniSkidi Project

const char* htmlHomePage PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=.9, maximum-scale=1, user-scalable=no">
  <style>
    .arrows {
      font-size:50px;
      color:grey;
    }
    td.button {
      background-color:black;
      border-radius:20%;
      box-shadow: 5px 5px #888888;
    }
    td.button:active {
      transform: translate(5px,5px);
      box-shadow: none; 
    }

    .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
         -khtml-user-select: none; /* Konqueror HTML */
           -moz-user-select: none; /* Firefox */
            -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }

    .slidecontainer {
      width: 100%;
    }

    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 20px;
      border-radius: 5px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }

    .slider:hover {
      opacity: 1;
    }
  
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 40px;
      height: 40px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 40px;
      height: 40px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }

    </style>
  
  </head>
  <body class="noselect" align="center" style="background-color:white">
     
          <div class="slidecontainer">
    <label for="powerSwitch" style="font-size: 20px;">HorizontalScreen:</label>
    <input type="checkbox" id="powerSwitch" class="power-switch" onchange='sendButtonInput("Switch", 0)'>
    </div> 
    <h1 style="color: black;text-align:center;">MINISKIDI</h1>
    
    <table id="mainTable" style="width:400px;margin:auto;table-layout:fixed" CELLSPACING=10>
      <tr>
        <td></td>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","1")'onmousedown='sendButtonInput("MoveCar","1")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8679;</span></td>
        <td></td>
      </tr>
      <tr>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","3")'onmousedown='sendButtonInput("MoveCar","3")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8678;</span></td>
        <td class="button"></td>    
        <td class="button" ontouchstart='sendButtonInput("MoveCar","4")'onmousedown='sendButtonInput("MoveCar","4")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8680;</span></td>
      </tr>
      <tr>
        <td></td>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","2")'onmousedown='sendButtonInput("MoveCar","2")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8681;</span></td>
        <td></td>
      </tr>
      <tr/>
      <tr/>
      <tr/><tr/>
      <tr>
        <td class="button" ontouchstart='sendButtonInput("MoveCar","6")'onmousedown='sendButtonInput("MoveCar","6")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8678;</span></td>
        <td class="button" ontouchstart='sendButtonInput("Light","6")'onmousedown='sendButtonInput("Light","6")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#9788;</span></td>  
        <td class="button" ontouchstart='sendButtonInput("MoveCar","5")'onmousedown='sendButtonInput("MoveCar","5")'onmouseup='sendButtonInput("MoveCar","0")' ontouchend='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8680;</span></td>
      </tr>
      <tr/>
      <tr/>
      <tr/><tr/>
      <tr>
        <td style="text-align:left;font-size:25px"><b>Bucket:</b></td>
        <td colspan=2>
         <div class="slidecontainer">
            <input type="range" min="10" max="180" value="90" class="slider" id="Bucket" oninput='sendButtonInput("Bucket",value)'>
          </div>
        </td>
      </tr>  
            <tr/>
      <tr/>
      <tr/><tr/> 
      <tr>
        <td style="text-align:left;font-size:25px"><b>AUX:</b></td>
        <td colspan=2>
         <div class="slidecontainer">
            <input type="range" min="10" max="180" value="90" class="slider" id="AUX" oninput='sendButtonInput("AUX",value)'>
          </div>
        </td>
      </tr> 

    </table>
  
    <script>
      var webSocketCarInputUrl = "ws:\/\/" + window.location.hostname + "/CarInput";      
      var websocketCarInput;
      const auxSlider = document.getElementById('AUX');
      const bucketSlider = document.getElementById('Bucket');
      
      function initCarInputWebSocket() 
      {
        websocketCarInput = new WebSocket(webSocketCarInputUrl);
        websocketCarInput.onclose   = function(event){setTimeout(initCarInputWebSocket, 2000);};
        websocketCarInput.onmessage = function(event){};        
      }
      
      function sendButtonInput(key, value) 
      {
       var data = key + "," + value;
       websocketCarInput.send(data);
      }
      function handleKeyDown(event) {
        if (event.keyCode === 38) {
            sendButtonInput("MoveCar", "1");
        }
        if (event.keyCode === 40)
        {
          sendButtonInput("MoveCar", "2");
        }
        if (event.keyCode ===37)
        {
          sendButtonInput("MoveCar", "3");
        }
        if (event.keyCode ===39)
        {
          sendButtonInput("MoveCar", "4");
        }
        if (event.keyCode === 87)
        {
          sendButtonInput("MoveCar", "5");
        }
        if (event.keyCode === 83)
        {
          sendButtonInput("MoveCar", "6");
        }
        if(event.keyCode === 69)
        {
          auxSlider.value = parseInt(auxSlider.value) + 5; // You can adjust the increment value as needed
          sendButtonInput("AUX",auxSlider.value);
      // Trigger the 'input' event on the slider to update its value
          auxSlider.dispatchEvent(new Event('input'));
        }
        if(event.keyCode === 68)
        {
          auxSlider.value = parseInt(auxSlider.value) - 5; // You can adjust the increment value as needed
          sendButtonInput("AUX",auxSlider.value);
      // Trigger the 'input' event on the slider to update its value
          auxSlider.dispatchEvent(new Event('input'));
        }
        if(event.keyCode === 81)
        {
          bucketSlider.value = parseInt(bucketSlider.value) + 5; // You can adjust the increment value as needed
          sendButtonInput("Bucket",bucketSlider.value);
      // Trigger the 'input' event on the slider to update its value
          bucketSlider.dispatchEvent(new Event('input'));
        }
        if(event.keyCode === 65)
        {
          bucketSlider.value = parseInt(bucketSlider.value) - 5; // You can adjust the increment value as needed
          sendButtonInput("Bucket",bucketSlider.value);
      // Trigger the 'input' event on the slider to update its value
          bucketSlider.dispatchEvent(new Event('input'));
        }
        }
      function handleKeyUp(event) {
        if (event.keyCode === 37 || event.keyCode === 38 || event.keyCode === 39 || event.keyCode === 40 || event.keyCode === 87 || event.keyCode === 83) {
            sendButtonInput("MoveCar", "0");
        }
    }  
      
  
      window.onload = initCarInputWebSocket;
      document.getElementById("mainTable").addEventListener("touchend", function(event){
        event.preventDefault()
      });
      document.addEventListener('keydown', handleKeyDown);
      document.addEventListener('keyup', handleKeyUp); 
           
    </script>
  </body>    
</html>
)HTMLHOMEPAGE";
