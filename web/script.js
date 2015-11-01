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

	//POINTS :
  	var triangle_vertex=[
                            -1,-1,-1,
                            0,0,0,
                            1,-1,-1,
                            1,0,0,
                            1,1,-1,
                            1,1,0,
                            -1,1,-1,
                            0,1,0,
                            -1,-1,1,
                            0,0,1,
                            1,-1,1,
                            1,0,1,
                            1,1,1,
                            1,1,1,
                            -1,1,1,
                            0,1,1	
  						]; 
    var TRIANGLE_VERTEX	= GL.createBuffer ();
    GL.bindBuffer(GL.ARRAY_BUFFER, TRIANGLE_VERTEX);
    GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(triangle_vertex), GL.STATIC_DRAW);

	//FACES :
    var triangle_faces = [
                            0,1,2,
                            0,2,3,

                            4,5,6,
                            4,6,7,

                            0,3,7,
                            0,4,7,

                            1,2,6,
                            1,5,6,

                            2,3,6,
                            3,7,6,

                            0,1,5,
                            0,4,5
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