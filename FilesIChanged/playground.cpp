#include "playground.h"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

//https://stackoverflow.com/questions/50975196/is-it-possible-to-change-the-color-of-an-object-with-a-variable-in-opengl
//change color

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Include chrono for sleep command
#include <chrono>
#include <thread>

#include <common/shader.hpp>

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  //brauche ich für jedes object einen vertex buffer die restlichen objekte bewegen sich gleich haben 
  // aber einen anderen start punkt
  //neuer code: ( project does not work with this)
  bool vertexbufferForObjectAInitialized = initializeVertexbufferForObjectA();
  if (!vertexbufferForObjectAInitialized) return -1;


  //color
  rS = 0.0;
  gS = 1.0;
  bS = 1.0;

  rB = 1.0;
  gB = 0.0;
  bB = 1.0;

  rSt = 0.0;
  gSt = 0.0;
  bSt = 0.0;


  bool vertexbufferForObjectBInitialized = initializeVertexbufferForObjectB();
  if (!vertexbufferForObjectBInitialized) return -1;

  bool vertexbufferForObjectCInitialized = initializeVertexbufferForObjectC();
  if (!vertexbufferForObjectCInitialized) return -1;

 /* bool vertexbufferForObjectB = initializeVertexbuffer();
  if (!vertexbufferForObjectB) return -1;

  bool vertexbufferForObjectC = initializeVertexbuffer();
  if (!vertexbufferForObjectC) return -1;*/

  //vertexbuffer: VBO stores the vertex data (triangle vertices)
  //                VBO is an array of data (mostly floats)
  // copy all data in a buffer so we dont have to draw one by one 
  //buffer stores data in the graphics hardware memory
  //= more efficient to use gpu


  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
  //this creates an GLSL program for the shaders so we can use them

  //brauch ich mehr shader wenn models verschieden bewegen will

  // Get a handle for our "MVP" and "MV" uniforms and update them for initalization 
  MatrixIDMV = glGetUniformLocation(programID, "MV");

  //parameters of glGetUniformLocation(Program object, *name: points to a string the name of the uniform variables)
    //returns integer that represents the location of a specific uniform variable within a program object
  // name must be in an active 
  // => returns programID?

  //send transformation to currently bound shader 
  //each model will have different mvp matrixes 
  MatrixID = glGetUniformLocation(programID, "MVP");


  //function to move 
  updateMVPTransformation(0,0,0);

  //initialize pose variables
  curr_x = 0;
  curr_y = 0;
  curr_angle = 0;
  int timer = 0;

  //neuer code:
  //position second object Done
  xA = 25.5f;
  yA = 0.0f;
  zA = 0.0f;

  //position third object Done
  xB = -135.5f;
  yB = 25.0f;
  zB = 0.0f;

  //position fourth object  TODO
  xC = -0.0f;
  yC = 0.0f;
  zC = 0.0f;
  //bis hier
  // 
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  //needed to store fragments their z valuee in depht buffer => is used clear depht buffer before each 
                                                                    //frame by GL_Depth_buffer_bit

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}



void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

 
  GLint myUniformLocation = glGetUniformLocation(programID, "rgbColor");
  glUniform3f(myUniformLocation, rB, gB, bB)/* some floating point value here */;


  // Update the variables for movement / rotation if a key was pressed
  if (glfwGetKey(window, GLFW_KEY_W)) curr_y+=2.0;
  else if (glfwGetKey(window, GLFW_KEY_S)) curr_y-=2.0;
  else if (glfwGetKey(window, GLFW_KEY_A)) curr_x-=2.0;
  else if (glfwGetKey(window, GLFW_KEY_D)) curr_x+=2.0;
  //else if (glfwGetKey(window, GLFW_KEY_R)) curr_angle += 0.5;

  bool dangerZone = false;

  if (curr_x <= xB + 50 || curr_x >= xA + 75 || curr_y < 0)             //-135.5f; = xA add number for bunny head when its in the danger zone get red
  {
      rB = 1.0;
      gB = 0.0;
      bB = 0.0;
      dangerZone = true;
  }
  if (curr_x > xB + 50 && curr_x < xA + 75 && curr_y > 0)             //change color back to normal when bunny is out of danger zone
  {
      rB = 1.0;
      gB = 0.0;
      bB = 1.0;
      dangerZone = false;
  }
  if (curr_y > 99 && dangerZone == false)               //change color when bunny is at a save spot
  {
      rB = 0.0;
      gB = 1.0;
      bB = 0.7;
  }

  timer++;
  if (timer >= 5) {
  //change color:
  rSt = rSt + 0.1;
  if (rSt >= 1) 
  {
      rSt = 0.5;
  }
  gSt = gSt + 0.2;
  if (gSt >= 1)
  {
      gSt = 0.5;
  }  
  bSt = bSt + 0.3;
  if (bSt >= 1)
  {
      bSt = 0.5;
  }
  timer = 0;
  }

  //for the other models it would be:
  /*
        y = y-1  (only from above to bottom then)
        if y = end window 
            delete model add new model with the same start points 
            or change change ý value of the model = same effect
  */

  yA = yA - 1;
  if (yA <= -300) 
  {
      yA = 0;
  }

  yB = yB - 1.7;
  if (yB <= -350)
  {
      yB = 0;
  }

  curr_angle += 0.05;

  // Update the MVP transformation with the new values
  updateMVPTransformation(curr_x, curr_y, 0.0);

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
  glVertexAttribPointer(
    1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Draw the triangle ! fishA
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle
  updateMVPTransformation(xA, yA, 0);
  glUniform3f(myUniformLocation, rS, gS, bS)/* some floating point value here */;

  //  updateMVPTransformation(xA + 1, yA +1 , 0 + 1);


  //new code (copy down the code from above for our next vertex buffer):
  // 
    // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectA[0]);

  //bind buffer (array buffer) to buffer binding point (vertexbuffer)
  //bind name (2.parameter) to target(1.parameter)

  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectA[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  //color?
  // 
  // 


  // Draw the triangle ! //fish
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferForObjectA_size); // 3 indices starting at 0 -> 1 triangle
  updateMVPTransformation(xB, yB, 0);

  //new code (copy down the code from above for our next vertex buffer):
  // 
    // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectB[0]);

  //bind buffer (array buffer) to buffer binding point (vertexbuffer)
  //bind name (2.parameter) to target(1.parameter)

  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectB[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );
  // Draw the triangle ! //fish
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferForObjectB_size); // 3 indices starting at 0 -> 1 triangle



  updateMVPTransformation(xC, yC, curr_angle);
  glUniform3f(myUniformLocation, rSt, gSt, bSt)/* some floating point value here */;

  //new code (copy down the code from above for our next vertex buffer):
  // 
    // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform and also the "MV" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDMV, 1, GL_FALSE, &MV[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectC[0]);

  //bind buffer (array buffer) to buffer binding point (vertexbuffer)
  //bind name (2.parameter) to target(1.parameter)

  glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );

  // 2nd attribute buffer : normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectC[1]);
  glVertexAttribPointer(
      1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
  );
  // Draw the triangle ! //fish
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferForObjectC_size); // 3 indices starting at 0 -> 1 triangle




  //maybee copy this 2 more times 
  //end code

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

//no need to change something
bool initializeWindow()
{
  // Initialise GLFW test
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Example: 3D Model with basic shading", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  return true;
}

bool updateMVPTransformation(float posX, float posY, float angle)
{
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);

  // Camera matrix (modify this to let the camera move)
  glm::mat4 View = glm::lookAt(
    glm::vec3(0, 0, 300), // Camera is at (0,0,0), in World Space
    glm::vec3(0, 0, 0), // and looks at the origin
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix, example with 3 parameters (curr_angle, curr_x, curr_y)
  glm::mat4 Model = glm::mat4(1.0f); //start with identity matrix
  //change rotation:
  Model = glm::rotate(Model, angle, glm::vec3(-0.0f, -1.0f, -0.50f)); //apply orientation (last parameter: axis)
  Model = glm::translate(Model,glm::vec3(posX, posY, 0.0f)); //apply translation
  
  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
  MV = View * Model; //We also need MV in the shader to transform the light position

  return true;
}

bool initializeVertexbuffer()
{
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  //create vertex and normal data
  std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
  std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

  //std::vector< glm::vec3 > vertices2 = std::vector< glm::vec3 >();
  //std::vector< glm::vec3 > normals2 = std::vector< glm::vec3 >();

  loadSTLFile(vertices, normals, "C:/temp/bunny-decimated.stl", 0, 0);
  //loadSTLFile(vertices2, normals2, "C:/temp/Shark.stl");
  vertexbuffer_size = vertices.size() * sizeof(glm::vec3);
  
  glGenBuffers(2, vertexbuffer); //generate two buffers, one for the vertices, one for the normals
  
  //fill first buffer (vertices)
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //fill second buffer (normals)
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  return true;
}


//again some code (I hope this works)
//we have to initilaize our other0 Vbuffer
bool initializeVertexbufferForObjectA()
{
    /*zum bewegen glm:mat4 = model matrix
    * model = einheitsmatrix
    * matrix als uniform in shader vertexshader
    * mat4 u_model;
    * u_model + vec4(position)
    * unifrm location abfragen 
    * int modelMatrixLocation = GLCALL(glGetUniformLocation(shader.getshaderId(), u_model));
    */
    glGenVertexArrays(1, &VertexArrayForObjectAID);
    glBindVertexArray(VertexArrayForObjectAID);

    //create vertex and normal data
    //std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    //std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

    std::vector< glm::vec3 > vertices= std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals= std::vector< glm::vec3 >();

    glm::vec3 triangleNormal = glm::vec3(xA,
        yA,
        zA);

    vertices.push_back(vec3(xA,yA,0.0));
    normals.push_back(triangleNormal);

    loadSTLFile(vertices, normals, "C:/temp/Shark.stl", xA, yA);
    vertexbufferForObjectA_size = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbufferForObjectA); //generate two buffers, one for the vertices, one for the normals

    //glGenBuffers(n, buffers) n = number of buffers, buffers = array in which buffer object names are stored 

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectA[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectA[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}


//we have to initilaize our other0 Vbuffer
bool initializeVertexbufferForObjectB()
{

    glGenVertexArrays(1, &VertexArrayForObjectBID);
    glBindVertexArray(VertexArrayForObjectBID);

    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();


    loadSTLFile(vertices, normals, "C:/temp/Shark.stl", -100, 0);
    vertexbufferForObjectB_size = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbufferForObjectB); //generate two buffers, one for the vertices, one for the normals

    //glGenBuffers(n, buffers) n = number of buffers, buffers = array in which buffer object names are stored 

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectB[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectB[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}


bool initializeVertexbufferForObjectC()
{

    glGenVertexArrays(1, &VertexArrayForObjectCID);
    glBindVertexArray(VertexArrayForObjectCID);

    std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
    std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();


    loadSTLFile(vertices, normals, "C:/temp/star.stl", 0, -100);
    vertexbufferForObjectC_size = vertices.size() * sizeof(glm::vec3);

    glGenBuffers(2, vertexbufferForObjectC); //generate two buffers, one for the vertices, one for the normals

    //glGenBuffers(n, buffers) n = number of buffers, buffers = array in which buffer object names are stored 

    //fill first buffer (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectC[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //fill second buffer (normals)
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferForObjectC[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return true;
}




bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteVertexArrays(1, &VertexArrayForObjectAID);
  glDeleteVertexArrays(1, &VertexArrayForObjectBID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

void loadSTLFile(std::vector< glm::vec3 >& vertices, 
                 std::vector< glm::vec3 >& normals, 
                 std::string stl_file_name,
                 float startPointX,
                 float startPointY)
{
  stl::stl_data info = stl::parse_stl(stl_file_name);
  std::vector<stl::triangle> triangles = info.triangles;
  for (int i = 0; i < info.triangles.size(); i++) {
    stl::triangle t = info.triangles.at(i);
    glm::vec3 triangleNormal = glm::vec3(t.normal.x, 
                                         t.normal.y, 
                                         t.normal.z);
    //add vertex and normal for point 1:
    vertices.push_back(glm::vec3(t.v1.x + startPointX, t.v1.y + startPointY, t.v1.z));
    normals.push_back(triangleNormal);
    //add vertex and normal for point 2:
    vertices.push_back(glm::vec3(t.v2.x + startPointX, t.v2.y + startPointY, t.v2.z));
    normals.push_back(triangleNormal);
    //add vertex and normal for point 3:
    vertices.push_back(glm::vec3(t.v3.x + startPointX, t.v3.y + startPointY, t.v3.z));
    normals.push_back(triangleNormal);
  }
}
