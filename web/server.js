
var http = require('http');
var url  = require('url');
var fs	 = require('fs');
var io	= require('socket.io');

var server = http.createServer(function(request,response){

				var path = url.parse(request.url).pathname;
				console.log("A request was made for " + path);	
				if(path =="/")
					path = "/index.html";

				fs.readFile(__dirname + path, function(error,data){
					if(error){
						response.writeHead(404);
						response.write("Hmmm cant find file " + path);
						response.end();
					}else{
						response.writeHead(200,{"Content-Type":"text/html"});
						response.write(data,"utf8");
						response.end();
					}
				});	
			

			});

server.listen(8001);
var websocket = io.listen(server);
websocket.on('connection', function(socket){
	for(var i=0; i<10; i++)
	 socket.emit('message', {'message': 'hello world'+i});
});
