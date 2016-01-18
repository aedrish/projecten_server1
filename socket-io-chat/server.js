var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.port || 8887;
var startTijd, eindTijd;
server.listen(port, function() {
	console.log('Server listening at port %d', port);
});


io.on('connection', function(socket) {
	startTijd = new Date().getTime();
	console.log("connctie");
	socket.on('message', function( data) {
		eindTijd = new Date().getTime();
		console.log("ontvangen na connectie in " + (eindTijd - startTijd));
		var obj = JSON.stringify(data);
		console.log("received message: "  + obj);
		console.log("sending");
		io.send(JSON.parse(obj));
	});
	
	socket.on('disconnect', function() {
		eindTijd = new Date().getTime();
		console.log("connected: " + (eindTijd - startTijd));
		console.log('disconnected');	
	});
});

