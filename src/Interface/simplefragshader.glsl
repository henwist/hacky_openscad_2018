#version 100
//Working example of changing colors depending on x,y,z,w coordinates.


#ifdef GL_ES
  precision mediump float;
//  precision mediump vec3;
//  precision mediump vec4;
//  precision mediump mat4;
#endif

//////////not used attributes from openscad shader:
       uniform vec4 color1, color2;
       varying vec3 tp, tr, tmp;
       varying float shading;

//////////

////Example working - and compiles - but objects are semi-transparent.

varying vec4 wsPosition;  // wsPosition  in world space
varying vec4 wsNormal;  // surface normal in world space
varying vec4 texel;
varying float alphaF;

uniform mat4 mv_inv;

uniform float shiningFactor;
/*uniform*/ vec4 cameraPos = vec4(0.0, 40.0/*/4.0*/, -80.0/*/4.0*/, 1.0);


vec4 ambientLight = vec4(2.0*0.4, 2.0*0.3, 2.0*0.2, 1.0);

struct Light
{
  vec4 pos; //position in world space
  vec4 dir; //direction of light beams
  vec4 diffuse;
  vec4 specular;
  
  float spotOpeningAngle; //in degrees, 0 -> no light can zeep through.
  
  float linAttenuation;
  float quadAttenuation;
};

const float scaleLightPos = 16.0*2.0;


const Light lightZero = Light(
  scaleLightPos*vec4(0.0, 6.0, 12.0, 1.0/scaleLightPos), //w:0 -> directional light, w:1 -> positional light
  vec4(0.0, 0.0, 0.0, 0.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  
  45.0,
  
  1.0,
  2.0
);


const Light lightOne = Light(
  scaleLightPos*vec4(1.0, -12.0, 0.0, 0.0/scaleLightPos), //w:0 -> directional light, w:1 -> positional light
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  vec4(0.1,  0.1,  0.1, 1.0),
  
  80.0,
  
  1.0,
  0.0
);

const Light lightTwo = Light(
scaleLightPos*vec4(0.0,  -1.0,  -2.0, 0.0), //w:0 -> directional light, w:1 -> positional light
  vec4(0.0, 1.0, 0.0, 0.0), //dir
  vec4(0.25,  0.25,  0.25, 1.0), //diffuse
  vec4(0.25,  0.25,  0.25, 1.0), //specular
  
  180.0,
  
  1.0,
  0.0
);

const Light lightThree = Light(
scaleLightPos*vec4(0.0,  1.0,  2.0, 0.0), //w:0 -> directional light, w:1 -> positional light
  vec4(0.0, -1.0, 0.0, 0.0), //dir
  vec4(0.25,  0.25,  0.25, 1.0), //diffuse
  vec4(0.25,  0.25,  0.25, 1.0), //specular
  
  180.0,
  
  1.0,
  0.0
);

const Light lightFour = Light(
  scaleLightPos*vec4(0.0,  1.0,  1.0, 0.0), //w:0 -> directional light, w:1 -> positional light
  vec4(0.0, 0.0, 1.0, 0.0), //dir
  vec4(2.0*0.25,  2.0*0.25,  2.0*0.25, 1.0), //diffuse
  vec4(0.25,  0.25,  0.25, 1.0), //specular
  
  180.0,
  
  1.0,
  0.0
);


  
struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shiningFactor;
};



const float scale = .70;
const Material mtrl = Material(
  vec4(scale*1.0,scale* 0.4, scale*0.4, 1.0),
  vec4(scale*1.0, scale*0.8, scale*0.8, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  100.205
);

const int countLights = 1;
Light lights[countLights];



vec4 ambientLightCalc(const Material mtrl, vec4 ambientLight)
{
  return mtrl.ambient * ambientLight;
}




vec4 diffuseLightCalc(const Material mtrl, Light light, vec4 viewingVector, vec4 objectNormal, vec4 lightVector) //vectors in relation to object viewed
{
  return max(0.0, /*dot(objectNormal, lightVector))*/ dot(reflect(lightVector, objectNormal), viewingVector)) * light.diffuse * mtrl.diffuse; 
}



vec4 specularLightCalc(const Material mtrl, Light light, vec4 viewingVector, vec4 objectNormal, vec4 lightVector)
{	
  float specularLightAmount = dot(reflect(lightVector, objectNormal), viewingVector); //how much of the specular reflected light in the obj normal goes to the viewer

  float whitenessFactor = pow(max(0.0, specularLightAmount), mtrl.shiningFactor);;
  
  vec4 specularLight = mtrl.specular * light.specular * whitenessFactor;
  
  return  dot(objectNormal, lightVector) < 0.0 ? vec4(0.0, 0.0, 0.0, 1.0) : specularLight; //If specular goes in the wrong direction, then return black.
}

float attenuation(Light light, vec4 lightVector) //lightVector in regard to wsPosition of fragment
{
  float lightDistance = length(lightVector);

  float dist = normalize(light.linAttenuation * lightDistance + light.quadAttenuation * lightDistance * lightDistance);

  float atten = 0.0;

  dist = dist < 0.0000001 ? 0.0000001 : dist; //to avoid 1.0 / bignumber = approx infinity ...

  atten = 1.0 / dist;
  
  return light.pos.w < 0.01 ? 1.0 : atten; //directional light have 1.0 attenuation that is it does not interfere.
}


void main()
{
  lights[0] = lightZero;
  //lights[1] = lightOne;
//   lights[2] = lightTwo;
//   lights[3] = lightThree;
//   lights[4] = lightFour;
  
  lights[0].pos = /*normalize*/(lightZero.pos * mv_inv); //if the mv matrix has change, then undo all operations for the light position - we want it to be static in position in space.
  //lights[1].pos = /*normalize*/(lightOne.pos  * mv_inv); 
//   lights[2].pos = /*normalize*/(lightTwo.pos  * mv_inv);
//   lights[3].pos = /*normalize*/(lightThree.pos  * mv_inv);
//   lights[4].pos = /*normalize*/(lightFour.pos  * mv_inv);

  vec4 totalLightReflection = ambientLightCalc(mtrl, ambientLight);
  vec4 diffuseLightReflection  =  vec4(0.0, 0.0, 0.0, 0.0);
  vec4 specularLightReflection =  vec4(0.0, 0.0, 0.0, 0.0);
  float atten = 1.0;
  vec4 lightVector = vec4(0.0, 0.0, 0.0, 0.0);
  //vec4 lightVectorAtt = vec4(0.0, 0.0, 0.0, 0.0);
  vec4 viewingVector = normalize(cameraPos * mv_inv - wsPosition);
  
  for(int lightIndex=0; lightIndex < countLights; lightIndex++)
  {
    //lightVector = /*normalize*/(wsPosition - lights[lightIndex].pos);
    lightVector = /*normalize*/lights[lightIndex].pos.w > 0.99 ? (wsPosition - lights[lightIndex].pos) : wsPosition - lights[lightIndex].dir * mv_inv;
    atten = attenuation(lights[lightIndex], lightVector);
  
    //diffuseLightReflection += max(0.0, /*dot(objectNormal, lightVector))*/ dot(reflect(lightVector, wsNormal), viewingVector)) * lights[lightIndex].diffuse * mtrl.diffuse;   

#ifndef GL_ES //early  shader model 2 hardware cannot coope with  too many hardware register - thus this code cannot run on those platforms.
    diffuseLightReflection += atten * diffuseLightCalc(mtrl,
						  lights[lightIndex],
  						 viewingVector, 
						  wsNormal, 
						  normalize(lightVector));
  
    specularLightReflection += atten * specularLightCalc(mtrl,
						    lights[lightIndex],
						    viewingVector, //normalize(cameraPos * mv_inv) - wsPosition,
						    wsNormal,
						    normalize(lightVector));
#endif // GL_ES
//     totalLighting = texel.xyz * (totalLighting + diffuseReflection + specularReflection);
//   totalLightReflection =  vec4(vec4(texel.xyz, 1.0) * (totalLightReflection + diffuseLightReflection + specularLightReflection)); //works good

   }

	
  totalLightReflection = /*atten **/ /*texel.xyz *  texel **/ (totalLightReflection + diffuseLightReflection + specularLightReflection);

 // gl_FragColor = /*texel **/ vec4(totalLightReflection.xyz, alphaF) + vec4(totalLightReflection.xyz, alphaF);
//   gl_FragColor = /*normalize*/(texel * vec4(totalLightReflection.xyz, totalLightReflection.w /*+ alphaF*/)) + /*normalize*/(vec4(totalLightReflection.xyz, totalLightReflection.w /*+ alphaF*/));

  if(__VERSION__  <= 100)
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

  if( 110 <= __VERSION__)
    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
