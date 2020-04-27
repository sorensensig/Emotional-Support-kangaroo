// The following code has been adapted from Bryan Jennings 
// post on how to record and play audio in JavaScript [1].

const recordAudio = () => {
  return new Promise(resolve => {
    navigator.mediaDevices.getUserMedia({ audio: true })
      .then(stream => {
        const mediaRecorder = new MediaRecorder(stream);
        const audioChunks = [];

        mediaRecorder.addEventListener("dataavailable", event => {
          audioChunks.push(event.data);
        });

        const start = () => {
          mediaRecorder.start();
        };

        const stop = () => {
          return new Promise(resolve => {
            mediaRecorder.addEventListener("stop", () => {
              const audioBlob = new Blob(audioChunks);
              const audioUrl = URL.createObjectURL(audioBlob);
              const audio = new Audio(audioUrl);
              const play = () => {
                audio.play();
              };

              resolve({ audioBlob, audioUrl, play });
            });

            mediaRecorder.stop();
          });
        };

        resolve({ start, stop });
      });
  });
};

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

        let audio;
        // Reading info from arduino, then sending that info to the server tagging as "message"
        parser.on('data', line => {
            switch(line){
                case "play":
                    audio.play();
                    break;
            }
        });

        socket.on('audio', payload => {
            // send rgb values file to Arduino
            audio = payload.audio;
            port.write(payload.colour);
        });
        // For debuggin purposes
        
    });
}

// end of adapted code [1]