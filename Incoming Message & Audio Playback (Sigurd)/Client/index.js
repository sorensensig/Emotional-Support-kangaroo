// This has been adapted from team member Marie Thorsen code [1]

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
        parser.on('data', line => {
            switch(line){
                case "play ":
                    // send play 
                    socket.emit('message', line);
                    break;
            }
        });

        socket.on('response', function (rgb) {
            // send rgb values file to Arduino
            port.write(rgb);
        });
        // For debuggin purposes
        
    });
}

// end of adapted code [1]