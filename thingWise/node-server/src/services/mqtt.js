var mqtt = require('mqtt');

var MqttHandler = function (host,  _port) {
    //[{ host: 'localhost', port: 1883 }, ... ]
    var options = {
        port: _port,
        clientId: 'thingwise_sever'  //Math.random().toString(16).substr(2, 8),
        //username: auth[0],
        //password: auth[1],
    };

    // Create a client connection
    this.mqttClient = mqtt.connect(host, options);
    
    this.mqttClient.on('connect', function(){
        this.subscribe('thingwise/#');
        //this.publish('citycar/KR/SEOUL/FKI/1', 'Hello mqtt');
        console.log('connected mqtt');    
    });
    
    this.mqttClient.on('message', function(topic, message){
        var topicArray = topic.split('/');
        console.log(topic.toString());
        console.log('topic depth > ' + topicArray.length);
        console.log(message.toString());
        /*
        if(topicArray.length === 5){
            try{
                var mqttMessageObj = JSON.parse(message);
                console.log(mqttMessageObj.carId);
                console.log(mqttMessageObj.type);
                console.log(mqttMessageObj.content.doorLock);
                
                if(mqttMessageObj.type && mqttMessageObj.type === 'status'){
                    carDatabase.addStatusToCar(mqttMessageObj.carId, mqttMessageObj.content, function(error, result){
                        console.log(result);
                    });    
                }
            }catch(e){
                
            }
            
        }
        */
    });
}

module.exports.MqttHandler = MqttHandler;