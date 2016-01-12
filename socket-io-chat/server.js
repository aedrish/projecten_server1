var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.port || 8887;

server.listen(port, function() {
	console.log('Server listening at port %d', port);
});


io.on('connection', function(socket) {
	console.log("connctie");
	socket.on('message', function( data) {
		var obj = JSON.stringify(data);
		console.log("received message: "  + obj);
		console.log("sending");
		io.send(JSON.parse(obj));
	});
	
	socket.on('disconnect', function() {
		console.log('disconnected');	
	});
});

