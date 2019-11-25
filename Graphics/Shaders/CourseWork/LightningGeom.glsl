#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float particleSize = 5.0;

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

in Vertex {
    vec2 texCoord;
    vec4 colour;
} IN [];

out Vertex {
    vec2 texCoord;
    vec4 colour;
} OUT ;
void main () {
    for (int i = 0; i < gl_in.length (); ++ i ) {
        OUT.colour = IN[i].colour;
        // top right
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += particleSize;
        gl_Position.y += particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (1, 0);

        EmitVertex ();
        // Then we do the other vertices of the quad ...
        // Top Left
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= particleSize;
        gl_Position.y += particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (0, 0);
        EmitVertex ();
        // bottom right
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += particleSize;
        gl_Position.y -= particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (1, 1);
        EmitVertex ();
        // bottom Left
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= particleSize;
        gl_Position.y -= particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (0, 1);
        EmitVertex();

        EndPrimitive();

        // top right
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z += particleSize;
        gl_Position.y += particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (1, 0);

        EmitVertex ();
        // Then we do the other vertices of the quad ...
        // Top Left
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z -= particleSize;
        gl_Position.y += particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (0, 0);
        EmitVertex ();
        // bottom right
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z += particleSize;
        gl_Position.y -= particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (1, 1);
        EmitVertex ();
        // bottom Left
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z -= particleSize;
        gl_Position.y -= particleSize;
        gl_Position = projMatrix * viewMatrix * (modelMatrix * vec4(gl_Position.xyz, 1));
        OUT.texCoord = vec2 (0, 1);
        EmitVertex();

        EndPrimitive();
    }
}