// The following code has been adapted by team member Sigurd, 
// based on the original work of Bryan Jennings post on how 
// to record and play audio in JavaScript [1].

// const recordAudio = () => {
//   return new Promise(resolve => {
//     navigator.mediaDevices.getUserMedia({ audio: true })
//       .then(stream => {
//         const mediaRecorder = new MediaRecorder(stream);
//         const audioChunks = [];

//         mediaRecorder.addEventListener("dataavailable", event => {
//           audioChunks.push(event.data);
//         });

//         const start = () => {
//           mediaRecorder.start();
//         };

//         const stop = () => {
//           return new Promise(resolve => {
//             mediaRecorder.addEventListener("stop", () => {
//               const audioBlob = new Blob(audioChunks);
//               const audioUrl = URL.createObjectURL(audioBlob);
//               const audio = new Audio(audioUrl);
//               const play = () => {
//                 audio.play();
//               };

//               resolve({ audioBlob, audioUrl, play });
//             });

//             mediaRecorder.stop();
//           });
//         };

//         resolve({ start, stop });
//       });
//   });
// };

// This has been adapted from team member Marie Thorsen code [2]

const io = require('socket.io-client');
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')

const portNum = 'COM5';

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
          // let recorder;
          let audio = "no work";
      
          //(async () => {
          //  recorder = await recordAudio();
          //})();

          line = parseInt(line);

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
                    const colour = "255,0,255";
                    let payload = {audio, colour};
                    console.log("Sending");
                    socket.emit('audio', payload);
                    console.log("Sent");
                    break;
            }
            
        });
        
        // let audio = "no work";
        // const colour = "255,0,255";
        // let payload = { audio, colour};
        // console.log("Sending");
        // socket.emit('audio', payload);
        // console.log("Sent");
        // For debuggin purposes
        
    });
}

// end of adapted code [2]