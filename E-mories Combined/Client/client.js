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
    
  const socket = io.connect('http://supparoo.uqcloud.net');
  socket.on('connect', () => {
    console.log("Connected");

    // let audio;
    // Reading info from arduino, then sending that info to the server tagging as "message"
    let counter = 0;
    const colours = ["255,0,0", "0,255,0", "0,0,255", "255,0,255", "255,255,0", "0,255,255"];
    // Reading info from arduino, then sending that info to the server tagging as "message"
    parser.on('data', line => {
      // let recorder;
    let audio = "no work";
    let colour = "";
    
    if(line.includes("message:")){
      let delimiter = line.indexOf(":", 0);
      colour = line.substring(delimiter, line.length-1);
    }
    

    let payload = {audio, colour};
    socket.emit('audio', payload);

    //(async () => {
    //  recorder = await recordAudio();
    //})();

    /*line = parseInt(line);

    switch(line){
      case 1:
        //recorder.start();
        console.log("IS RECORDING!");
        break;
      case 0:
        console.log("STAAAAAAAHP!");
        //(async () => {
          //audio = await recorder.stop();
        //})();
        counter++;
        if (counter >= colours.length){
          counter = 0;
        }
        let colour = colours[counter];
        let payload = {audio, colour};
        console.log("Sending");
        socket.emit('audio', payload);
        console.log("Sent");
        break;
      }*/
    });
    
    // Change the above switch statement with the code underneath when all prototypes are put together
    // socket.emit('audio', payload);

    socket.on('audio', payload => {
      let arr = covertRGB(payload.colour);
      port.write(arr);
    });

    socket.on('response', function (msg) {
      port.write(msg);
    });
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