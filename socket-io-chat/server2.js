var http = require('http'),
io = require('socket.io');

var app = http.createServer();
app.listen(8887);

var io = io.listen(app), nicknames = {};

io.sockets.on('connection' , function(socket) {
	socket.on('message', function(msg) {
		socket.emit("message", msg);
	});

	socket.on('disconnect', function() {
	});
});

