

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;
uniform sampler2D maskTex;
uniform vec2 texSize;
uniform vec2 maskTexSize;
uniform vec2 c;//spot center in world space
uniform float maskBaseBrightness;

void main() {
    
    vec2 curPixelPos=vec2(v_texCoord.x,1.0-v_texCoord.y)*texSize;//curPixel pos in world space
    
    vec2 maskTexLUPos=c+0.5*vec2(-maskTexSize.x,maskTexSize.y);//maskTex's left up corner pos in world space
    
    vec2 curMaskTexCoord=(curPixelPos-maskTexLUPos)/maskTexSize;//cur texture coord in maskTex space
    curMaskTexCoord.t=-curMaskTexCoord.t;//because origin of texture space at LU, so we need to reverse the t coord
    
    vec2 curTexCoord=vec2(v_texCoord.s,1.0-v_texCoord.t);//cur texture coord in CC_Texture0 space. because CC_Texture0 is generate from render to texture, so we need 1-v_texCoord.t
    
    vec3 texColor=texture2D(CC_Texture0,curTexCoord).rgb;//cur pixel's color of CC_Texture0
    vec2 maskTexColor=texture2D(maskTex,curMaskTexCoord).rg;//cur pixel's color of maskTex
    
    float maskBrightness=maskTexColor.r*2.0+maskBaseBrightness;//use maskTexColor's r component and maskBaseBrightness to calculate mask brightness
    
    float orangeBrightness=maskTexColor.g;//use maskTexColor's g component as orange brightness
    vec3 orangeColor=vec3(1.0,1.0,1.0)-orangeBrightness*(vec3(1.0,1.0,1.0)-vec3(2.0,0.7,0.0));//calculate orangleColor: interpolate between orange color and white color using orangeBrightness as the weight

    
    gl_FragColor=vec4(texColor*orangeColor*maskBrightness,1.0);//compose the final fragment color
    
}

