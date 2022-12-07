/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs

// all varying inputs
// TODO #F3: create the varying input

in vec3 theColor;

// all fragment outputs
// TODO #D: create the color output

out vec4 fragColorOut;

void main() {
    //*****************************************
    //******* Final Color Calculations ********
    //*****************************************
    
    // TODO #E: specify the fragment color
    fragColorOut = vec4(theColor, 1.0);

    // TODO #F4: write out the varying color

}
