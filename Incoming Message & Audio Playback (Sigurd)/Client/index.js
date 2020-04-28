// This has been adapted from team member Marie Thorsen code [1]

const io = require('socket.io-client');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

// const portNum = process.argv.slice(2)[0];
const portNum = "/dev/cu.usbmodem14301";

if (!portNum) {
    console.log("Need USB port for arduino");
    process.exitCode = 1;

} else {
  console.log('running');
    const port = new SerialPort(portNum, {
        baudRate: 115200
    });
    const parser = new Readline();
    port.pipe(parser);
    
    var socket = io.connect('http://supparoo.uqcloud.net');
    socket.on('connect', function () {
        console.log("Connected");

        // let audio;
        // Reading info from arduino, then sending that info to the server tagging as "message"
        parser.on('data', line => {
          console.log(line);
            switch(line){
                case "play":
                    // audio.play();
                    break;
            }
        });

        socket.on('audio', payload => {
          let arr = covertRGB(payload.colour);
          port.write(arr);
        });
        // For debuggin purposes
    });
}

const covertRGB = input => {
  let arr = [];

  let firstDelimiter = 0;
  let lastDelimiter = 0;

  for(let i = 0; i < 3; i++) {
    lastDelimiter = input.indexOf(",", firstDelimiter);
    switch(i) {
      case 0:
        arr[0] = parseInt(input.substring(firstDelimiter, lastDelimiter));
        break;
      case 1:
        arr[1] = parseInt(input.substring(firstDelimiter, lastDelimiter));
        break;
      case 2:
        arr[2] = parseInt(input.substring(firstDelimiter, input.length+1));
        break;
    }
    firstDelimiter = lastDelimiter+1;
  }
  return arr;
}
// end of adapted code [1]