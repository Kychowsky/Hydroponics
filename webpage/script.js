const brokerHost = "10.0.0.249";
const brokerPort = 9001;
const clientId   = "webclient-" + Date.now();

// Default WS path is "/"
const client = new Paho.Client(brokerHost, brokerPort, "/", clientId);

//connection lost code- to be reworked
client.onConnectionLost = response => {
    console.error('Connection lost:', response.errorMessage);
    document.getElementById('status').textContent = 'Disconnected';
    ['tdsValue','tempValue','phValue'].forEach(id => document.getElementById(id).textContent = '--');
};

//when mqtt message comes in!
client.onMessageArrived = message => {
    const topic = message.destinationName;
    const payload = message.payloadString;
    switch(topic) {
    case 'home/sensors/tds':
        document.getElementById('tdsValue').textContent = payload;
        break;
    case 'home/sensors/temp':
        document.getElementById('tempValue').textContent = payload;
        break;
    case 'home/sensors/ph':
        document.getElementById('phValue').textContent = payload;
        break;
    case 'home/errors':
        document.getElementById('errorDisplay').textContent = payload;
    }
};

//when webpage successfully connects to
function onConnect() {
    console.log('Connected to MQTT');
    document.getElementById('status').textContent = 'Connected';
    client.subscribe('home/sensors/tds');
    client.subscribe('home/sensors/temp');
    client.subscribe('home/sensors/ph');
}

function onFailure(err) {
    console.error('Connection failed:', err.errorMessage);
    document.getElementById('status').textContent = 'Connection Error';
    setTimeout(() => client.connect({ useSSL: false, onSuccess: onConnect, onFailure }), 5000);
}

// Connect to broker
client.connect({
    useSSL: false,
    onSuccess: onConnect,
    onFailure: onFailure
});