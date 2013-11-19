

#include "maskedSceneSprite.h"
bool CmaskedSceneSprite::init(CCTexture2D *texture,string maskTexFileName){
    this->initWithTexture(texture);
    this->maskTex=CCTextureCache::sharedTextureCache()->addImage(maskTexFileName.c_str());
    CCSize texSize=this->getContentSize();
    //mask shader
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("mask.fsh").c_str())->getCString();
		CCGLProgram* pProgram = new CCGLProgram();
		pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
        //bind attribute
		pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        //link  (must after bindAttribute)
		pProgram->link();
        //get cocos2d-x build-in uniforms
		pProgram->updateUniforms();
        //get my own uniforms
		map<string,GLint> myUnifoMap;
        myUnifoMap["texSize"] = glGetUniformLocation(pProgram->getProgram(),"texSize");
        myUnifoMap["maskTexSize"] = glGetUniformLocation(pProgram->getProgram(),"maskTexSize");
        myUnifoMap["c"] = glGetUniformLocation(pProgram->getProgram(),"c");
        myUnifoMap["maskTex"] = glGetUniformLocation(pProgram->getProgram(),"maskTex");
        //make program
        program.myUnifoMap=myUnifoMap;
        program.setProgram(pProgram);
        //program can be released
        pProgram->release();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
	//check gl error
	CHECK_GL_ERROR_DEBUG();
    
    
    return true;
}
void CmaskedSceneSprite::draw(){
    CCSize texSize=this->getContentSize();
    CCSize maskTexSize=maskTex->getContentSize();
    //----change shader
    this->setShaderProgram(program.getProgram());
    ccGLEnable(m_eGLServerState);
    //pass values for cocos2d-x build-in uniforms
    getShaderProgram()->use();
    getShaderProgram()->setUniformsForBuiltins();
    //pass values for my own uniforms
    float texSize_c[2]={texSize.width,texSize.height};
    glUniform2fv(program.myUnifoMap["texSize"],1,texSize_c);
    float maskTexSize_c[2]={maskTexSize.width,maskTexSize.height};
    glUniform2fv(program.myUnifoMap["maskTexSize"],1,maskTexSize_c);
    float c_c[2]={c.x,c.y};
    glUniform2fv(program.myUnifoMap["c"], 1, c_c);
    //pass texture attach point id to sampler uniform
    glUniform1i(program.myUnifoMap["maskTex"],1);
    //attach texture to texture attach point
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, maskTex->getName());
    glActiveTexture(GL_TEXTURE0);//back to GL_TEXTURE0
    //call father draw
    CCSprite::draw();
    
}