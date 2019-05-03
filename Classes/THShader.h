#pragma once


#include "cocos2d.h"

using namespace cocos2d;

class THShader : public GLProgram {
public:
    THShader();
    ~THShader();
    
    static THShader* createWithDefines(const std::string& vShaderFilename, const std::string& fShaderFilename, const std::string& vDefines, const std::string& fDefines);
    
    bool initWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename, const std::string& vDefines, const std::string& fDefines);

    bool initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray, const std::string& vDefines, const std::string& fDefines);
    
    bool compileShaderTH(GLuint* shader, GLenum type, const GLchar* source, const std::string& defines);

};