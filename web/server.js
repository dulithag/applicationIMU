
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

var rawBuffer = new Buffer(6);
var rawPt = 0;

function process(buff, socket){

	for(var i=0; i<buff.length;i++){
		if(buff.readUInt8(i) == 0x5c){
			//console.log("Found 5C");
			rawPt++;
			continue;
		}
		if(buff[i]== 0x6e){
			//console.log("Found 6E");
			rawPt=0;
			//console.log("raw buffer :"+rawBuffer.toString('hex'));
			var x_data = rawBuffer.readInt16BE(0);
			var y_data = rawBuffer.readInt16BE(2);
			var z_data = rawBuffer.readInt16BE(4);
			//console.log(rawBuffer);
			console.log("X=" + x_data + ", Y=" + y_data + ", Z=" + z_data );
	 		socket.emit('message', {'x':x_data,'y':y_data,'z':z_data});
			continue;
		}
		if(rawPt>5)
				console.log("THE WORLD FALLS APPART");
		rawBuffer[rawPt] = buff[i];
		rawPt++;
	}	
}


server.listen(8001);
var websocket = io.listen(server);
websocket.on('connection', function(socket){
								console.log("connection made");
	 							socket.emit('message', 'Opening Serial Port ...');
								var serialport = new SerialPort("/dev/ttyUSB0",{baudrate:115200});

								serialport.on("open", function () {
		    						serialport.on('data', function(data) {
										var buff = new Buffer(data,'hex');
					    				//console.log('data received: ' + buff.toString('hex'));
										process(data,socket);
									});
								});
					}
			);


							
