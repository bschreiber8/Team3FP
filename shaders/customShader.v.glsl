/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs
// TODO #A: create the MVP uniform
uniform mat4 mvpMatrix;

// TODO #G1: create the time uniform
uniform float time;

// all attribute inputs
// TODO #B: create the position attribute

in vec3 vPosition;

// all varying outputs
// TODO #F1: create the varying output

out vec3 theColor;




void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************

    // TODO #G2: modify the vertex position

    vec3 newVertex = vPosition;
    if(vPosition.x > 0 && vPosition.y > 0 && vPosition.z > 0){
        newVertex = vPosition + 1.2 * ((sin(time)+1)/2) - 0.2;
    }

    // TODO #C: set the transformed vertex position

    gl_Position = mvpMatrix * vec4(newVertex,1.0);


    // TODO #F2: set our varying

    theColor = newVertex;

}
