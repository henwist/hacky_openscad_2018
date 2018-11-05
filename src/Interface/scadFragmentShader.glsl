       #version 100

#ifdef GL_ES
  precision mediump float;
//  precision mediump vec3;
//  precision mediump vec4;
//  precision mediump mat4;
#endif

       uniform vec4 color1, color2;
       varying vec3 tp, tr, tmp;
       varying float shading;
       void main() {
         gl_FragColor = vec4(color1.r * shading, color1.g * shading, color1.b * shading, color1.a);
         if (tp.x < tr.x || tp.y < tr.y || tp.z < tr.z)
           gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); //color2;
       }