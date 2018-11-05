#version 100

//Copyright(C) Henrik Wistbacka 2018
//Licence GPL version 2

#ifdef GL_ES
  precision mediump float;
//  precision mediump vec3;
//  precision mediump vec4;
//  precision mediump mat4;
#endif

////////not used attributes from openscad shader
       uniform mat4 modelViewProjectionMatrix;
       uniform mat3 normalMatrix;
       uniform vec3 lightSource;
       uniform float xscale, yscale;
       attribute vec3 pos_b, pos_c;
       attribute vec3 trig, mask;
       varying vec3 tp, tr;
       varying float shading;

/////////

attribute vec4 vertex;
attribute vec3 normal;

varying vec4 wsPosition;  // wsPosition in word space (ws)
varying vec4 wsNormal;  // wsNormal in world space
//varying vec4 texel;
varying float alphaF; //alpha value

uniform mat4 m, v, p; //model, view and projection matrices
 
uniform mat4 mv_inv; //inverted and transponed

uniform mat4 rotMatrix;

//uniform sampler2D mytexture2D;

uniform float alpha;

void main()
{
  wsPosition =  /*normalize*/(m * vertex);
  wsNormal = normalize(m * vec4(normal, 0.0));

  //texel = vec4(1.0, 1.0, 1.0, 1.0); //normalize(texture2D(mytexture2D, wsPosition.xy));


  alphaF = alpha;

  gl_Position = p * v * m * rotMatrix * vertex;
}
