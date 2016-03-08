#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       //The matrix that defines the transformation of the
                            //object we're rendering. In this assignment,
                            //this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  //The inverse transpose of the model matrix.
                            //This allows us to transform the object's normals properly
                            //if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    //The matrix that defines the camera's transformation.
                            //We've written a static matrix for you to use for HW2,
                            //but in HW3 you'll have to generate one yourself

uniform mat4 u_TransMats[100]; //Overall Transformation Matrices of Each Joint

uniform mat4 u_BindMats[100]; //Bind Matrices of Each Joint

uniform int u_selectedJoint; //If this ID == to joint ID in influence, color change by influence

in vec4 vs_Pos;//---------->The array of vertex positions passed to the shader
in vec4 vs_Nor;//---------->The array of vertex normals passed to the shader
in vec4 vs_Col;//---------->The array of vertex colors passed to the shader.

in vec2 weights; //Influences amounts
in ivec2 joint_IDs; //Influences ID's


out vec4 fs_Nor;//--------->The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;//---->The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Col;//--------->The color of each vertex. This is implicitly passed to the fragment shader.

const vec4 lightPos = vec4(5, 5, 3, 1); //The position of our virtual light, which is used to compute the shading of
                                        //the geometry in the fragment shader.

void main()
{
    fs_Col = vs_Col;//                          Pass the vertex color positions to the fragment shader
    fs_Nor = u_ModelInvTr * vs_Nor;//           Transform the geometry's normals

    vec4 modelposition;
    modelposition =  u_Model * ((weights.x * u_TransMats[joint_IDs.x] * u_BindMats[joint_IDs.x] * vs_Pos) +
            (weights.y * u_TransMats[joint_IDs.y] * u_BindMats[joint_IDs.y] * vs_Pos));//    Temporarily store the transformed vertex positions for use below


    fs_Col = vec4(joint_IDs[0]/4.0, joint_IDs[1]/4.0, 0, 1);

    if (u_selectedJoint != -1) {
         if (u_selectedJoint == joint_IDs[0]) {
             fs_Col = vec4(1, 1-(weights.x), 0, 1);

         } else if (u_selectedJoint == joint_IDs[1]) {
             fs_Col = vec4(1, 1-(weights.y),  0, 1);
         }
         else {
             fs_Col = vec4(0.2,0.2,0.2,1);
         }
    }

    //If the mesh is not bound, then color it white and just set it at its default location

    if (u_BindMats[13][3][0] == 0 ||  u_BindMats[13][3][1] == 0 || u_BindMats[13][3][2] == 0) {
        modelposition = u_Model * vs_Pos;
        fs_Col = vec4(1,1,1,1);//White if its trans mats & bind mats are 0's
    }


    fs_LightVec = lightPos - modelposition;//   Compute the direction in which the light source lies

    gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is used to render the final positions
                                             // of the geometry's vertices
}
