function createShaderProgram(GL){

	console.log("======= initialize Shaders =======") ;
  	var shader_vertex_source="													\n\
        attribute vec3 position; 	//the position of the point 				\n\
        attribute vec3 color;	  	//the color of the point					\n\
        uniform mat4 Pmatrix;													\n\																		\n\
        uniform mat4 Mmatrix;													\n\
        uniform mat4 Vmatrix;													\n\
        varying vec3 vColor; 													\n\
        void main(void) { 														\n\
        	gl_Position = Pmatrix * Vmatrix * Mmatrix *vec4(position, 1.);// and 1 is w				\n\
        	vColor = color;														\n\
        }";

  	var shader_fragment_source="\n\
        precision mediump float;\n\
        \n\
        \n\
        \n\
        varying vec3 vColor;													\n\
        void main(void) {														\n\
        	gl_FragColor = vec4(vColor, 1.); //black color						\n\
        }";

    var get_shader=function(source, type, typeString) {
    	var shader = GL.createShader(type);
        GL.shaderSource(shader, source);
        GL.compileShader(shader);
        if (!GL.getShaderParameter(shader, GL.COMPILE_STATUS)) {
        	alert("ERROR IN "+typeString+ " SHADER : " + GL.getShaderInfoLog(shader));
          	return false;
        }
        return shader;
    };

 	var shader_vertex	=	get_shader(shader_vertex_source, GL.VERTEX_SHADER, "VERTEX");
  	var shader_fragment	=	get_shader(shader_fragment_source, GL.FRAGMENT_SHADER, "FRAGMENT");

	var SHADER_PROGRAM	=	GL.createProgram();
  	GL.attachShader(SHADER_PROGRAM, shader_vertex);
  	GL.attachShader(SHADER_PROGRAM, shader_fragment);

	GL.linkProgram(SHADER_PROGRAM);

	var _position = GL.getAttribLocation(SHADER_PROGRAM, "position");
	var _color	 = GL.getAttribLocation(SHADER_PROGRAM, "color");
	var _Pmatrix = GL.getUniformLocation(SHADER_PROGRAM, "Pmatrix");
	var _Mmatrix = GL.getUniformLocation(SHADER_PROGRAM, "Mmatrix");
	var _Vmatrix = GL.getUniformLocation(SHADER_PROGRAM, "Vmatrix");

  	GL.useProgram(SHADER_PROGRAM);
  	
  	this.attribute_position = _position;
  	this.attribute_color 	= _color;
  	this.uniform_Pmatrix 	= _Pmatrix;
  	this.uniform_Mmatrix 	= _Mmatrix;
  	this.uniform_Vmatrix 	= _Vmatrix;
}

function createBuffers(GL){

  	console.log("======= Fill ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER =======") ;

	var triangle_vertex=[
                            -0.7,-0.5,-1,		//z- bot left G    0
                            0,1,0,
							-0.7,-0.5,-1,		//z- bot left B    1
                            0,0,1,
							-0.7,-0.5,-1,		//z- bot left Y    2
                            1,1,0,

                            0.7,-0.5,-1,		//z- bot right G   3
                            0,1,0,
                            0.7,-0.5,-1,		//z- bot right B   4
                            0,0,1,
                            0.7,-0.5,-1,		//z- bot right C   5
                            0,1,1,

                            0.7,0.5,-1,			//z- top right G  6
                            0,1,0,
                            0.7,0.5,-1,			//z- top right R  7
                            1,0,0,
                            0.7,0.5,-1,			//z- top right C  8
                            0,1,1,

                            -0.7,0.5,-1,		//z- top left G   9
                            0,1,0,
                            -0.7,0.5,-1,		//z- top left R  10
                            1,0,0,
                            -0.7,0.5,-1,		//z- top left Y  11
                            1,1,0,
                           
                          //4 
                            -0.7,-0.5,1,		//z+ bot left P    12
                            1,0,1,
							-0.7,-0.5,1,		//z+ bot left B    13
                            0,0,1,
							-0.7,-0.5,1,		//z+ bot left Y    14
                            1,1,0,
                           
                          //5 
                            0.7,-0.5,1,			//z+ bot right P   15
                            1,0,1,
                            0.7,-0.5,1,			//z+ bot right B   16
                            0,0,1,
                            0.7,-0.5,1,			//z+ bot right C   17
                            0,1,1,
                           
                          //6 
                            0.7,0.5,1,			//z+ top right p  18
                            1,0,1,
                            0.7,0.5,1,			//z+ top right R  19
                            1,0,0,
                            0.7,0.5,1,			//z+ top right C  20
                            0,1,1,
                           
                          //7 
                            -0.7,0.5,1,		//z+ top left G   21
                            1,0,1,
                            -0.7,0.5,1,		//z+ top left R  22
                            1,0,0,
                            -0.7,0.5,1,		//z+ top left Y  23
                            1,1,0
  		];

    var TRIANGLE_VERTEX	= GL.createBuffer ();
    GL.bindBuffer(GL.ARRAY_BUFFER, TRIANGLE_VERTEX);
    GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(triangle_vertex), GL.STATIC_DRAW);

	//FACES :
 var triangle_faces = [
                            0,3,6,
                            0,6,9,

                            12,15,18,
                            12,18,21,

                            2,11,23,
                            2,14,23,

                            5,8,20,
                            5,17,20,

                            7,10,19,
                            10,22,19,

                            1,4,16,
                            1,13,16
    		];
  
  	var TRIANGLE_FACES = GL.createBuffer();
  	GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, TRIANGLE_FACES);
  	GL.bufferData(GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(triangle_faces), GL.STATIC_DRAW);

	this.vertex_id = TRIANGLE_VERTEX;
	this.faces_id = TRIANGLE_FACES;
} 



function main() {
  	console.log("Start Main Application") ;

  	var CANVAS	= 	document.getElementById("mycanvas");
  	CANVAS.width	=	window.innerWidth;
  	CANVAS.height	=	window.innerHeight;
	document.onkeydown = handleKeyDown;
  	console.log("======= Get WebGL Context =======") ;
  	try {
  	  var GL = CANVAS.getContext("experimental-webgl", {antialias: true});
  	} catch (e) {
  	  alert("You are not webgl compatible :(") ;
  	  return false;
  	}

	//Create Buffers
	var shaderProgram = new createShaderProgram(GL);
	var buffers = new createBuffers(GL);


  	var PROJMATRIX=LIBS.get_projection(40, CANVAS.width/CANVAS.height, 1, 100);
  	var MOVEMATRIX=LIBS.get_I4();
  	var VIEWMATRIX=LIBS.get_I4();

	function handleKeyDown(event){
    	switch (String.fromCharCode(event.keyCode)){
        	case "W" :
			    LIBS.rotateX(MOVEMATRIX, Math.PI/180);
        		break;
        	case "S" :
			    LIBS.rotateX(MOVEMATRIX, -(Math.PI/180));
        		break;
        	case "D" :
			    LIBS.rotateY(MOVEMATRIX, Math.PI/180);
                break;
        	case "A" :
			    LIBS.rotateY(MOVEMATRIX, -(Math.PI/180));
                break;
        };
    }

 	LIBS.translateZ(VIEWMATRIX, -15);

	//Draw	
	GL.clearColor(0.0, 0.0, 0.0, 0.0);
 	GL.enable(GL.DEPTH_TEST);
  	GL.depthFunc(GL.LEQUAL);

  	GL.clearDepth(1.0);
  
	var dAngle	=	.5;//0.001*(time-time_old);
    //LIBS.rotateY(MOVEMATRIX, dAngle);
    //LIBS.rotateX(MOVEMATRIX, dAngle);
  	var time_old=0;	

	var animate=function(time) {
		
		
    	time_old	=	time;	
    	
		
    	GL.viewport(0.0, 0.0, CANVAS.width, CANVAS.height);
    	GL.clear(GL.COLOR_BUFFER_BIT);

		//Load Uniforms
		GL.uniformMatrix4fv(shaderProgram.uniform_Pmatrix, false, PROJMATRIX);
		GL.uniformMatrix4fv(shaderProgram.uniform_Mmatrix, false, MOVEMATRIX);
		GL.uniformMatrix4fv(shaderProgram.uniform_Vmatrix, false, VIEWMATRIX);

		//Load Attributes
     	GL.bindBuffer(GL.ARRAY_BUFFER, buffers.vertex_id);
	  	GL.enableVertexAttribArray(shaderProgram.attribute_position);
	  	GL.enableVertexAttribArray(shaderProgram.attribute_color);
    	GL.vertexAttribPointer(shaderProgram.attribute_position	, 3, GL.FLOAT, false,4*(3+3),0) ;
    	GL.vertexAttribPointer(shaderProgram.attribute_color	, 3, GL.FLOAT, false,4*(3+3),3*4) ;

		//Load Draw elements Arary
    	GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, buffers.faces_id);
    	GL.drawElements(GL.TRIANGLES, 36, GL.UNSIGNED_SHORT, 0);	

    	GL.flush();

    	window.requestAnimationFrame(animate);
  	};

  	animate(0);


  	console.log("End Main Application") ;
};
