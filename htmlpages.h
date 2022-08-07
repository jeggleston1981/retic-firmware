const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<style>
:root {
  --background: #dfdcdc;
  --text-color: #000000;
  --border: #aaabb8;
  --lowlight: #464866;
  --highlight: #aaabb8;
}
* {
    box-sizing: border-box;
    padding: 0;
    margin: 0;
    font-family: 'Courier New', Courier, monospace;
}
body {
    background-color: var(--background);
}
nav ul{
    overflow: hidden;
    background-color: var(--highlight);
    height: 40px;
}
nav li {
    list-style-type: none;
    float: left;

}
.row li {
    list-style-type: none;
    float: right;
    margin-left: 20px;
    margin-right: 20px;
}
nav button {
    border-style: none;
    border-right: 1px solid var(--lowlight);
    color: var(--text-color);
    border-color: var(--lowlight);
    background-color: var(--highlight);
    font-size: 1em;
    height: 40px ;
    width: 100px;
}
button {
    border: 1px solid var(--lowlight);
    height: 60px;
    width: 60px;
    color: var(--text-color);
    font-weight: bold;
    background-color: var(--highlight);
}
.row{
    height: 30px;
    display: inline;
}
.spacer {
    display: inline-block;
    height: 30px;
}
.container {
    display: flex;
    flex-direction: column;
    row-gap: 5px;
}

</style>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Easy Reticulation</title>
</head>
<body>
    <nav>
    <ul>
        <li class="row"><button>HOME</button></li>
        <li class="row"><button>PROGRAM</button></li>
        <li class="row" style="float: right;"><button style="border-left: 1px solid var(--lowlight);">07:45 TU</button></li>
    </ul>
    </nav>
    <div class="container">
    <div class="spacer" style="padding: 10px 0px 0px 20px;"><span id="s1">Station 1 - 10 min</span></div>
    <div class="row">
    <ul>
        <li style="float:left;"><button id="s1button" onclick="toggle(this)">ON</button></li>
        <li><button>R</button></li>
        <li><button>T</button></li>
    </ul>
    </div><br>
    <div class="spacer" style="padding: 10px 0px 0px 20px;"><span id="s2">Station 2 - 10 min</span></div>
    <div class="row">
        <li style="float:left;"><button id="s2button" onclick="toggle(this)">ON</button></li>
        <li><button>R</button></li>
        <li><button>T</button></li>
    </ul>
    </div>
    </div>
</body>
<script>
var server = 'ws://' + window.location.hostname + '/ws';
var websocket;
window.addEventListener('load', onLoad);
function initWebSocket(){
    console.log('Starting new websocket connection...');
    websocket = new WebSocket(server);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onMessage(event) {
    console.log(event.data);
}
function onOpen(event) {
    console.log('Connection open');
}
function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
function onLoad(event) {
    initWebSocket();
}

function toggle(element){
    var id = element.id;
    console.log("button clicked");
    var obj = {};
    obj[id] = "toggle";
    console.log(obj);
    websocket.send(JSON.stringify(obj));
}
</script>
</html>
)rawliteral";
