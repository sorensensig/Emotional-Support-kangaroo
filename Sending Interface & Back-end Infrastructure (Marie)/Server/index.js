var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function (req, res) {
    //res.sendFile(__dirname + '/index.html');
    res.send('<h1>Hello world!</h1>');
});

io.on('connection', function (socket) {
    console.log("User connected");

    socket.on('message', function (msg) {
        console.log('message: ' + msg);
        socket.broadcast.emit('message', msg);
        io.emit('message', "received");
    });

    socket.on('audio', payload => {
      socket.broadcast.emit('audio', payload);
      // send confirmation?
    });

    socket.on('disconnect', function () {
        console.log("User disconnected");
    });
});

http.listen(3000, function () {
    console.log("Listening on *:3000")
});