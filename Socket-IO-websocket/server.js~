var port = 8887;
var io = require('socket.io').listen(port, function() {
	console.log("listening on port: " + port);
});

io.sockets.on('connection', function (socket) {
	socket.on('set nickname', function (name) {
		socket.set('nickname', name, function() {
			socket.emit('ready');
		});
	});

	socket.on('msg', function() {
		socket.get('nickname', function (err, name) {
			console.log('Chat message by ', name);
		});
	});
});

