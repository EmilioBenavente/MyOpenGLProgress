#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

void main()
{
  gl_Position = gl_in[0].gl_Position + vec4(-45.5, 0.0,
					    0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(45.5, 0.0,
					    0.0, 0.0);
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + vec4(45.5, 45.8,
					    0.0, 0.0);
  EmitVertex();

  
  EndPrimitive();
}
