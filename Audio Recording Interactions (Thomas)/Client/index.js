// The following code has been adapted by team member Sigurd, 
// based on the original work of Bryan Jennings post on how 
// to record and play audio in JavaScript [1].

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

// This has been adapted from team member Marie Thorsen code [2]

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
          let recorder;
          let audio;
      
          (async () => {
            recorder = await recordAudio();
          })();

            switch(line){
                case "record":
                    recorder.start();
                    break;
                case "stopRecord":
                    (async () => {
                        audio = await recorder.stop();
                    })();
                    const colour = '255,0,0';
                    let payload = { audio, colour};
                    socket.emit('audio', payload);
                    break;
            }
            
        });
        // For debuggin purposes
        
    });
}

// end of adapted code [2]