#ifndef SHADER_H
#define SHADER_H

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);
GLuint LoadShaders(const char * vertex_file, const char * fragment_file);

#endif
