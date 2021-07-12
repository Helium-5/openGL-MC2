#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Headers/shader_m.h>
#include <Headers/camera.h>

#include <iostream>
#include <vector>
#include <iomanip>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vshader.vs", "fshader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    unsigned int VBO1, VAO1, EBO1;
    unsigned int VBO2, VAO2, EBO2;
    
    const GLfloat PI = 3.14159265358979323846f;
    const int Y_SEGMENTS = 100;
    const int X_SEGMENTS = 100;

    {
        std::vector<float> sphereVertices;
        std::vector<int> sphereIndices;

        /*2-Calculate the vertices of the sphere*/
        //Generate the vertices of the ball
        for (int y = 0; y <= Y_SEGMENTS; y++)
        {
            for (int x = 0; x <= X_SEGMENTS; x++)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                float zn = ySegment;

                sphereVertices.push_back(xPos);
                sphereVertices.push_back(yPos);
                sphereVertices.push_back(zPos);

                sphereVertices.push_back(zn);
                sphereVertices.push_back(1 - zn);
                sphereVertices.push_back(0.0f);
            }
        }

        //Indices that generate the ball
        for (int i = 0; i < Y_SEGMENTS; i++)
        {
            for (int j = 0; j < X_SEGMENTS; j++)
            {
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }


        // world space positions of our cubes

        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO1);
        glGenBuffers(1, &EBO1);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(float), &sphereIndices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }
    {
        std::vector<float> sphereVertices;
        std::vector<int> sphereIndices;

        /*2-Calculate the vertices of the sphere*/
        //Generate the vertices of the ball
        for (int y = 0; y <= Y_SEGMENTS; y++)
        {
            for (int x = 0; x <= X_SEGMENTS; x++)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                float zn = ySegment;

                sphereVertices.push_back(xPos);
                sphereVertices.push_back(yPos);
                sphereVertices.push_back(zPos);

                sphereVertices.push_back(1);
                sphereVertices.push_back(1 - zn);
                sphereVertices.push_back(zn);
            }
        }

        //Indices that generate the ball
        for (int i = 0; i < Y_SEGMENTS; i++)
        {
            for (int j = 0; j < X_SEGMENTS; j++)
            {
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
                sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }


        // world space positions of our cubes

        glGenVertexArrays(1, &VAO2);
        glGenBuffers(1, &VBO2);
        glGenBuffers(1, &EBO2);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(float), &sphereIndices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }

    ourShader.use();



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO1);

        float distance = 8.0f;
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(-distance / 4, 0, -distance));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);


        glBindVertexArray(VAO2);
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(distance / 4, 0, -distance));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);

        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);

    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}