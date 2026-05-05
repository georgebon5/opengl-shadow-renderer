#version 330 core

// Θέση κορυφής (vertex) του πλανήτη
layout (location = 0) in vec3 aPos;

// Μετασχηματισμοί
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Ο πλανήτης μετασχηματίζεται κανονικά από local -> world -> view -> clip space
    // Δεν χρειάζεται φωτισμός εδώ, μόνο σωστή γεωμετρία
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}