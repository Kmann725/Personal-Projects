#version 460 core  
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;

void main()
{ 
    color = texture(texture, TexCoords);
    float average = (color.r + color.g + color.b) / 3.0f;
    color = vec4(average, average, average, 1.0);
}