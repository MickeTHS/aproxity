#include "THShader.h"

#include "cocos2d.h"

static const char * TH_COCOS2D_SHADER_UNIFORMS =
"uniform mat4 CC_PMatrix;\n"
"uniform mat4 CC_MVMatrix;\n"
"uniform mat4 CC_MVPMatrix;\n"
"uniform mat3 CC_NormalMatrix;\n"
"uniform vec4 CC_Time;\n"
"uniform vec4 CC_SinTime;\n"
"uniform vec4 CC_CosTime;\n"
"uniform vec4 CC_Random01;\n"
"uniform sampler2D CC_Texture0;\n"
"uniform sampler2D CC_Texture1;\n"
"uniform sampler2D CC_Texture2;\n"
"uniform sampler2D CC_Texture3;\n"
"//CC INCLUDES END\n\n";


THShader::THShader() : GLProgram() {

}

THShader::~THShader() {

}

THShader* THShader::createWithDefines(const std::string &vShaderFilename, const std::string &fShaderFilename, const std::string &vDefines, const std::string &fDefines) {
    auto ret = new (std::nothrow) THShader();
    if(ret && ret->initWithFilenames(vShaderFilename, fShaderFilename, vDefines, fDefines)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THShader::initWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename, const std::string& vDefines, const std::string& fDefines) {
    auto fileUtils = FileUtils::getInstance();
    std::string vertexSource = fileUtils->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(vShaderFilename));
    std::string fragmentSource = fileUtils->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(fShaderFilename));
    
    return initWithByteArrays(vertexSource.c_str(), fragmentSource.c_str(), vDefines, fDefines);
}

bool THShader::initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray, const std::string& vDefines, const std::string& fDefines) {
    _program = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();
    
    _vertShader = _fragShader = 0;
    
    if (vShaderByteArray) {
        if (!compileShaderTH(&_vertShader, GL_VERTEX_SHADER, vShaderByteArray, vDefines)) {
            CCLOG("TH: ERROR: Failed to compile vertex shader");
            return false;
        }
    }
    
    // Create and compile fragment shader
    if (fShaderByteArray) {
        if (!compileShaderTH(&_fragShader, GL_FRAGMENT_SHADER, fShaderByteArray, fDefines)) {
            CCLOG("TH: ERROR: Failed to compile fragment shader");
            return false;
        }
    }
    
    if (_vertShader) {
        glAttachShader(_program, _vertShader);
    }
    CHECK_GL_ERROR_DEBUG();
    
    if (_fragShader) {
        glAttachShader(_program, _fragShader);
    }
    
    _hashForUniforms.clear();
    
    CHECK_GL_ERROR_DEBUG();
    
    return true;
}

bool THShader::compileShaderTH(GLuint* shader, GLenum type, const GLchar* source, const std::string& defines) {
    GLint status;
    
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
        (type == GL_VERTEX_SHADER ? "precision mediump float;\n precision mediump int;\n" : "precision mediump float;\n precision mediump int;\n"),
#elif (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        defines.c_str(),
        (type == GL_VERTEX_SHADER
                ? "precision highp float;\n precision highp int;\n"
                : "precision mediump float;\n precision mediump int;\n"),
#endif
        TH_COCOS2D_SHADER_UNIFORMS,
        source};
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, nullptr);
    glCompileShader(*shader);
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    
    if (! status)
    {
        GLsizei length;
        glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
        GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
        
        glGetShaderSource(*shader, length, nullptr, src);
        CCLOG("cocos2d: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLOG("cocos2d: %s", getVertexShaderLog().c_str());
        }
        else
        {
            CCLOG("cocos2d: %s", getFragmentShaderLog().c_str());
        }
        free(src);
        
        return false;;
    }
    return (status == GL_TRUE);
}