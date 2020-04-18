const io = require('socket.io-client');
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')

const portNum = process.argv.slice(2)[0];

if (!portNum) {
    console.log("Need USB port for arduino");
    process.exitCode = 1;

} else {
    const port = new SerialPort(portNum, {
        baudRate: 115200
    });
    const parser = new Readline();
    port.pipe(parser);
    
    var socket = io.connect('http://supparoo.uqcloud.net');
    socket.on('connect', function () {
        console.log("Connected");
        // Reading info from arduino, then sending that info to the server tagging as "message"
        parser.on('data', line => socket.emit('message', line));
        
        // For debuggin purposes
        socket.on('response', function (msg) {
            port.write(msg);
        });
    });
}