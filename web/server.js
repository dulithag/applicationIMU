
var http = require('http');
var url  = require('url');
var fs	 = require('fs');
var io	= require('socket.io');
var SerialPort = require("serialport").SerialPort;

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
							console.log("connection made");
	 						socket.emit('message', {'message': 'Opening Serial Port ...'});
							var serialport = new SerialPort("/dev/ttyUSB0",{baudrate:115200});
							serialport.on("open", function () {
		    					serialport.on('data', function(data) {
					    			console.log('data received: ' + data);
	 								socket.emit('message', {'message': data});
								});
							});
					}
			);


							
