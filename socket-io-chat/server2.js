var cluster = require('cluster'),
    _portSocket = 8887,
    _portRedis = 6379,
    _HostRedis = 'localhost';
if (cluster.isMaster) {
    var server = require('http').createServer(),
        socketIO = require('socket.io').listen(server),
        redis = require('socket.io-redis');
    socketIO.adapter(redis({
        host: _HostRedis,
        port: _portRedis
    }));
    var numberOfCPUs = require('os').cpus().length;
    for (var i = 0; i < numberOfCPUs; i++) {
        cluster.fork();
    }
    cluster.on('fork', function(worker) {
        console.log('Worker %s creëeren', worker.id);
    });
    cluster.on('online', function(worker) {
        console.log('worker %s gecreëert', worker.id);
    });
    cluster.on('listening', function(worker, addr) {
        console.log('worker %s luisterd op %s:%d', worker.id, addr.address, addr.port);
    });
    cluster.on('disconnect', function(worker) {
        console.log('Worker %s disconnected', worker.id);
    });
    cluster.on('exit', function(worker, code, signal) {
        console.log('worker %s gestopt (%s)', worker.id, signal || code);
        if (!worker.suicide) {
            console.log('Recreëer worker %s', worker.id);
            cluster.fork();
        }
    });
}
if (cluster.isWorker) {
	var http = require('http');
    http.globalAgent.maxSockets = Infinity;
    var app = require('express')(),
        ent = require('ent'),
        fs = require('fs'),
        server = http.createServer(app).listen(_portSocket),
        socketIO = require('socket.io').listen(server),
        redis = require('socket.io-redis');
    socketIO.adapter(redis({
        host: _HostRedis,
        port: _portRedis
    }));
    socketIO.sockets.on('connection', function(socket, pseudo) {

		console.log("connctie");
		//console.log(cluster.worker.id);
		socket.on('message', function( data) {
			var obj = JSON.stringify(data);
			console.log("received message: "  + obj);
			console.log("sending");
			socket.send(JSON.parse(obj));
		});
	
		socket.on('disconnect', function() {
			console.log('disconnected');	
		});

    });
}
