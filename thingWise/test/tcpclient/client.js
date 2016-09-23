var meta = {
    program : 'C100K Client Socket',
    options : {
        parameters : [
            ['m', 'maxConn', 'max connection request', '50000'],
            ['h', 'host', 'server host', '192.168.99.100'],
            ['p', 'port', 'server port', '3000'],
            ['c', 'connInterval', 'connection interval', '1'],
            ['l', 'messageLength', 'message length', '0'],
            ['n', 'maxMessageEachConn', 'max message each conn', '0'],
            ['i', 'messageInterval', 'message interval', '1000']
        ]
    },
    usages : [
        [null, ['[m]', '[h]', '[p]', '[c]', '[l]', '[n]', '[i]'], null, 'start client', startClient]
    ]
};

function startClient(r) {
    var serverHost = r.parameters.host;
    var serverPort = r.parameters.port;
    var serverAddr = serverHost + ":" + serverPort;

    console.log('Client is Running! Server: ' + serverAddr);

    var messageLength = r.parameters.messageLength;
    var message = undefined;
    if (messageLength > 0) {
        var mg = require('./MessageGenerator');
        message = mg.string(messageLength);
    }

    var connectionRequestCount = 0;
    var connectedCount = 0;
    var disconnectedCount = 0;
    var messageSendCount = 0;
    var messageRecvCount = 0;

    var net = require('net');
    var connectTask = setInterval(function() {
        var client = new net.Socket();
        connectionRequestCount++;
        client.connect(serverPort, serverHost, function() {
            connectedCount++;

            if (messageLength > 0) {
                var messageCount = 0;
                var messageTask = setInterval(function() {
                    client.write(message);
                    messageSendCount++;
                    messageCount++;
                    if (messageCount == r.parameters.maxMessageEachConn) {
                        clearInterval(messageTask);
                    }
                }, r.parameters.messageInterval);
            }
        });

        client.on('data', function() {
            messageRecvCount++;
        });

        client.on('close', function() {
            connectedCount--;
            disconnectedCount++;
        });

        if (connectionRequestCount >= r.parameters.maxConn) {
            clearInterval(connectTask);
        }
    }, r.parameters.connInterval);

    var statTask = setInterval(function() {
        console.log('req:' + connectionRequestCount
                + ' conn:' + connectedCount + ' lost:' + disconnectedCount
                + ' send:' + messageSendCount + ' recv: ' + messageRecvCount);
    }, 1000);
}

var lineparser = require('lineparser');
var parser = lineparser.init(meta);
parser.parse(process.argv.slice(2));
