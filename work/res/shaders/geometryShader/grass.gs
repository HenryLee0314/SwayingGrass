#version 410 core
layout (lines) in;
// layout (line_strip, max_vertices = 2) out;
layout (triangle_strip, max_vertices = 4) out;


// uniform vec3 viewPos;

in vec3 tesFragPos[2];
in vec3 tesEw[2];
in vec3 tesEn[2];
// // in vec2 vsTexCoord[3];

//out vec3 fsFragPos;
// out vec2 fsTexCoord;
// out vec3 fsNormal;

out vec3 gsFragPos;
out vec3 gsEn;

 void main()
 {
    float scale = 0.01;
    gl_Position = gl_in[0].gl_Position + scale * vec4(tesEw[0], 0.0);
    gsFragPos = tesFragPos[0] + scale * tesEw[0];
    gsEn = tesEn[0];
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - scale * vec4(tesEw[0], 0.0);
    gsFragPos = tesFragPos[0] - scale * tesEw[0];
    gsEn = tesEn[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + scale * vec4(tesEw[1], 0.0);
    gsFragPos = tesFragPos[1] + scale * tesEw[1];
    gsEn = tesEn[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - scale * vec4(tesEw[1], 0.0);
    gsFragPos = tesFragPos[1] - scale * tesEw[1];
    gsEn = tesEn[0];
    EmitVertex();

    // gl_Position = gl_in[0].gl_Position;
    // EmitVertex();

    // gl_Position = gl_in[1].gl_Position;
    // EmitVertex();





    EndPrimitive();
}