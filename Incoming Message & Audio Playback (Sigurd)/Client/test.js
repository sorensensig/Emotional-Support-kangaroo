const puppeteer = require('puppeteer');

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

let recorder;
let audio;

(async () => {
  const browser = await puppeteer.launch({
    ignoreDefaultArgs: ['--mute-audio'],
  });
  
  const page = await browser.newPage();
  await page.evaluate(() => {
    (async () => {
      recorder = await recordAudio();
      recorder.start();
      console.log('recording');
      setTimeout(recorder => {
        (async () => {
          audio = await recorder.stop();
          console.log('stop recording');
          audio.play();
          console.log('play recording');
        })
      }, 3000);
    });
  });
});

while(true);

// const init = async () => {
//   recorder = await recordAudio();
// }

// const startRecording = () => {
//   recorder.start();
// }

// const stopRecording = async () => {
//   audio = await recorder.stop();
// }

// const playRecording = () => {
//   audio.play();
// }