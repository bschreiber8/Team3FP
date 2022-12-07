#include "Lab04Engine.hpp"

#include <CSCI441/objects.hpp>

#include <fstream>
#include <string>

//*************************************************************************************
//
// Public Interface

Lab04Engine::Lab04Engine()
         : CSCI441::OpenGLEngine(4, 1, 640, 480, "Lab04: GLSL Shaders") {
    _freeCam = new CSCI441::FreeCam();
    _objectIndex = 0;

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

Lab04Engine::~Lab04Engine() {
    delete _freeCam;
}

void Lab04Engine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;

            case GLFW_KEY_1:
            case GLFW_KEY_2:
            case GLFW_KEY_3:
            case GLFW_KEY_4:
            case GLFW_KEY_5:
            case GLFW_KEY_6:
            case GLFW_KEY_7:
                _objectIndex = key - GLFW_KEY_1;
                break;

            default: break; // suppress CLion warning
        }
    }
}

void Lab04Engine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void Lab04Engine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        // rotate the camera by the distance the mouse moved
        _freeCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                         (_mousePosition.y - currMousePosition.y) * 0.005f );
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void Lab04Engine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab04_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab04_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab04_engine_cursor_callback);
}

void Lab04Engine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                    // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black
}

void Lab04Engine::_setupShaders() {
    // TODO #09B: create our shader program!

    _customShaderProgramHandle = _createShaderProgram("shaders/customShader.v.glsl", "shaders/customShader.f.glsl");
    // TODO #10A: get our MVP uniform location

    _customShaderProgramUniformLocations.modelViewProjection = glGetUniformLocation(_customShaderProgramHandle, "mvpMatrix");

    // TODO #12B: get our time uniform location

    _customShaderProgramUniformLocations.time =  glGetUniformLocation(_customShaderProgramHandle, "time");


    // TODO #10B: get our position attribute location

    _customShaderProgramAttributeLocations.vPos = glGetAttribLocation(_customShaderProgramHandle, "vPosition");


    // LOOK HERE #3: needed to connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _customShaderProgramAttributeLocations.vPos );
}

// _setupBuffers() /////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
void Lab04Engine::_setupBuffers() {
    _model = new CSCI441::ModelLoader();
    _model->loadModelFile( "models/suzanne.obj" );
    // LOOK HERE #4
    _model->setAttributeLocations(_customShaderProgramAttributeLocations.vPos);
}

void Lab04Engine::_setupScene() {
    _freeCam->setPosition( glm::vec3(10.0f, 10.0f, 10.0f) );
    _freeCam->setTheta( 5.52f );
    _freeCam->setPhi( 0.9f );
    _freeCam->recomputeOrientation();
    _cameraSpeed = glm::vec2(0.25f, 0.02f);

    _objectIndex = 2;
}

//*************************************************************************************
//
// Engine Cleanup

void Lab04Engine::_cleanupShaders() {
    // LOOK HERE #4: we're cleaning up our memory again!
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    glDeleteShader(_customShaderProgramHandle);
}

void Lab04Engine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _model;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void Lab04Engine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {
    // TODO #11: use our shader program and send the MVP matrix
    glUseProgram(_customShaderProgramHandle);
    glm::mat4 mvpMtx = projMtx*viewMtx;
    glProgramUniformMatrix4fv(_customShaderProgramHandle, _customShaderProgramUniformLocations.modelViewProjection, 1, GL_FALSE, &mvpMtx[0][0]);

    // TODO #12C: send time to GPU

    glUseProgram(_customShaderProgramHandle);

    glProgramUniform1f(_customShaderProgramHandle, _customShaderProgramUniformLocations.time, glfwGetTime());


    // draw all the cool stuff!
    switch( _objectIndex ) {
        case 0: CSCI441::drawSolidTeapot( 2.0 );                                                   break;
        case 1: CSCI441::drawSolidCube( 4.0 );                                                break;
        case 2: CSCI441::drawSolidSphere( 3.0, 16, 16 );                           break;
        case 3: CSCI441::drawSolidTorus( 1.0, 2.0, 16, 16 );         break;
        case 4: CSCI441::drawSolidCone( 2.0, 4.0, 16, 16 );                  break;
        case 5: CSCI441::drawSolidCylinder( 2.0, 2.0, 4.0, 16, 16 );    break;
        case 6: _model->draw( _customShaderProgramHandle );                            break;
    }
}

void Lab04Engine::_updateScene() {
    // fly
    if( _keys[GLFW_KEY_SPACE] ) {
        // go backward if shift held down
        if( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
            _freeCam->moveBackward(_cameraSpeed.x);
        }
        // go forward
        else {
            _freeCam->moveForward(_cameraSpeed.x);
        }
    }
    // turn right
    if( _keys[GLFW_KEY_D] ) {
        _freeCam->rotate(_cameraSpeed.y, 0.0f);
    }
    // turn left
    if( _keys[GLFW_KEY_A] ) {
        _freeCam->rotate(-_cameraSpeed.y, 0.0f);
    }
    // pitch up
    if( _keys[GLFW_KEY_W] ) {
        _freeCam->rotate(0.0f, _cameraSpeed.y);
    }
    // pitch down
    if( _keys[GLFW_KEY_S] ) {
        _freeCam->rotate(0.0f, -_cameraSpeed.y);
    }
}

void Lab04Engine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 100].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 100.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _freeCam->getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void Lab04Engine::_readTextFromFile( const char* filename, char* &output ) {
    std::string buf = std::string("");
    std::string line;

    std::ifstream in( filename );
    while( getline(in, line) ) {
        buf += line + "\n";
    }
    output = new char[buf.length()+1];
    strncpy(output, buf.c_str(), buf.length());
    output[buf.length()] = '\0';

    in.close();
}

void Lab04Engine::_printLog( GLuint handle ) {
    int infoLogLength = 0;
    int maxLength;

    // check if the handle is to a vertex/fragment shader
    if( glIsShader( handle ) ) {
        glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    // check if the handle is to a shader program
    else {
        glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
    }

    // create a buffer of designated length
    char infoLog[maxLength];

    bool isShader;
    if( glIsShader( handle ) ) {
        // get the info log for the vertex/fragment shader
        glGetShaderInfoLog( handle, maxLength, &infoLogLength, infoLog );

        isShader = true;
    } else {
        // get the info log for the shader program
        glGetProgramInfoLog( handle, maxLength, &infoLogLength, infoLog );

        isShader = false;
    }

    // if the length of the log is greater than 0
    if( infoLogLength > 0 ) {
        // print info to terminal
        printf( "[INFO]: %s Handle %d: %s\n", (isShader ? "Shader" : "Program"), handle, infoLog );
    }
}

GLuint Lab04Engine::_compileShader( const char* filename, GLenum shaderType ) {
    char *shaderString;

    // LOK HERE #1: read in each text file and store the contents in a string
    _readTextFromFile( filename, shaderString );

    // TODO #01: create a handle to our shader

    GLuint shaderHandle = glCreateShader(shaderType);



    // TODO #02: send the contents of the shader to the GPU
    glShaderSource(shaderHandle, 1, (const char**) &shaderString, NULL);


    _printLog( shaderHandle );

    // we are good programmers so free up the memory used by each buffer
    delete [] shaderString;

    // TODO #03: compile the shader on the GPU
    glCompileShader(shaderHandle);

    _printLog( shaderHandle );

    return shaderHandle;
}

GLuint Lab04Engine::_createShaderProgram( const char *vertexShaderFilename, const char *fragmentShaderFilename ) {

    // compile each one of our shaders
    GLuint vertexShaderHandle   = _compileShader( vertexShaderFilename,   GL_VERTEX_SHADER   );
    GLuint fragmentShaderHandle = _compileShader( fragmentShaderFilename, GL_FRAGMENT_SHADER );

    // TODO #04: get a handle to a shader program
    GLuint shaderProgramHandle = glCreateProgram();

    // TODO #05: attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgramHandle, vertexShaderHandle);
    glAttachShader(shaderProgramHandle, fragmentShaderHandle);

    // TODO #06: link all the programs together on the GPU

    glLinkProgram(shaderProgramHandle);

    _printLog( shaderProgramHandle );

    // TODO #07: detach the shaders from the program
    glDetachShader(shaderProgramHandle, vertexShaderHandle);
    glDetachShader(shaderProgramHandle, fragmentShaderHandle);


    // TODO #08: delete the compiled shaders from GPU memory

    glDeleteShader(vertexShaderHandle);
    glDeleteShader(fragmentShaderHandle);



    // return handle
    return shaderProgramHandle;
}

//*************************************************************************************
//
// Callbacks

void lab04_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab04_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab04_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}