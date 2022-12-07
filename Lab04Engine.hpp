#ifndef LAB04_LAB04_ENGINE_HPP
#define LAB04_LAB04_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/ModelLoader.hpp>
#include <CSCI441/OpenGLEngine.hpp>

class Lab04Engine : public CSCI441::OpenGLEngine {
public:
    Lab04Engine();
    ~Lab04Engine();

    void run() final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

private:
    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;

    void _cleanupBuffers() final;
    void _cleanupShaders() final;

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx);
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static const GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;

    /// \desc the static fixed camera in our world
    CSCI441::FreeCam* _freeCam;
    /// \desc pair of values to store the speed the camera can move/rotate.
    /// \brief x = forward/backward delta, y = rotational delta
    glm::vec2 _cameraSpeed;

    /// \desc stores a loaded OBJ model
    CSCI441::ModelLoader* _model;

    /// \desc tracks which object to currently render
    GLuint _objectIndex;

    // TODO #09A: create a shader program handle data member
    GLuint _customShaderProgramHandle;  // this step was intentionally left done for you to prevent compiler error
                                        // you can see where it is already being used in _renderScene() case 6
    /// \desc stores locations of our shader program uniforms
    struct CustomShaderProgramUniformLocations {
        // LOOK HERE #2: stores location for MVP Uniform
        /// \desc location to send the precomputed MVP matrix to
        GLint modelViewProjection;
        // TODO #12A: store the time location
        GLfloat time;

    } _customShaderProgramUniformLocations;
    /// \desc stores locations of our shader program attributes
    struct CustomShaderProgramAttributeLocations {
        /// \desc location of where to send vertex positions
        GLint vPos;
    } _customShaderProgramAttributeLocations;

    /// \desc reads text from an external file and stores in a c-string
    /// \param [in] filename file to read contents from
    /// \param [out] output uninitialized c-string to write contents to - will allocate memory internally
    static void _readTextFromFile( const char* filename, char* &output );
    /// \desc checks GPU Shader logs and prints any messages to the screen
    /// \param handle Shader or Program handle
    static void _printLog( GLuint handle );
    /// \desc creates and compiles a given shader as written in an external file
    /// \param filename file to read shader from
    /// \param shaderType type of the shader contained in the file
    /// \returns shader handle
    static GLuint _compileShader( const char* filename, GLenum shaderType );
    /// \desc creates, compiles, and links a Vertex and Fragment Shader in to a complete Shader Program
    /// \param vertexShaderFilename file to read the vertex shader from
    /// \param fragmentShaderFilename file to read the fragment shader from
    /// \returns shader program handle
    static GLuint _createShaderProgram( const char *vertexShaderFilename, const char *fragmentShaderFilename );
};

void lab04_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void lab04_engine_cursor_callback(GLFWwindow *window, double x, double y);
void lab04_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

#endif // LAB04_LAB04_ENGINE_HPP
