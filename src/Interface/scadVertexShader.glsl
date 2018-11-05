//
//     Uniforms:
//       1 color1 - face color
//       2 color2 - edge color
//       7 xscale
//       8 yscale
// 
//     Attributes:
//       3 trig
//       4 pos_b
//       5 pos_c
//       6 mask
// 
//     Other:
//       9 width
//       10 height
// 
//     Outputs:
//       tp
//       tr
//       shading
//

       #version 100

#ifdef GL_ES
  precision mediump float;
//  precision mediump vec3;
//  precision mediump vec4;
//  precision mediump mat4;
#endif


       uniform mat4 modelViewProjectionMatrix;
       attribute vec4 vertex;
       attribute vec3 normal;
       uniform mat3 normalMatrix;
       uniform vec3 lightSource;
       uniform float xscale, yscale;
       attribute vec3 pos_b, pos_c;
       attribute vec3 trig, mask;
       varying vec3 tp, tr;
       varying float shading;
       void main() {
         vec4 p0 = modelViewProjectionMatrix * vertex;
         vec4 p1 = modelViewProjectionMatrix * vec4(pos_b, 1.0);
         vec4 p2 = modelViewProjectionMatrix * vec4(pos_c, 1.0);
         float a = distance(vec2(xscale*p1.x/p1.w, yscale*p1.y/p1.w), vec2(xscale*p2.x/p2.w, yscale*p2.y/p2.w));
         float b = distance(vec2(xscale*p0.x/p0.w, yscale*p0.y/p0.w), vec2(xscale*p1.x/p1.w, yscale*p1.y/p1.w));
         float c = distance(vec2(xscale*p0.x/p0.w, yscale*p0.y/p0.w), vec2(xscale*p2.x/p2.w, yscale*p2.y/p2.w));
         float s = (a + b + c) / 2.0;
         float A = sqrt(s*(s-a)*(s-b)*(s-c));
         float ha = 2.0*A/a;
         gl_Position = vertex; //p0;
         tp = mask * ha;
         tr = trig;
         vec3 normal, lightDir;
         normal = normalize(normalMatrix * normal);
         lightDir = normalize(lightSource);
         shading = 0.2 + abs(dot(normal, lightDir));
       }