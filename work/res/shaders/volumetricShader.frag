//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
uniform sampler2D texture0;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec3 vLight;

varying vec4 lightPos;
const int Quality = 64;

void main() {
vec2 radial_origin = vLight.xy;
vec2 radial_size = vec2(0.0,0.0);    // texel size
 
float radial_blur = 0.8;   // blur factor
float radial_bright = 0.2; // bright factor

  vec2 TexCoord = vTextureCoord0;
 
  vec4 SumColor = vec4(0.0, 0.0, 0.0, 0.0);
  TexCoord += radial_size * 0.5 - radial_origin;
 
  for (int i = 0; i < Quality; i++) 
  {
    float scale = 1.0 - radial_blur * (float(i) / float(Quality));
    SumColor += texture2D(texture0, TexCoord * scale + radial_origin);
  }
 
  gl_FragColor = SumColor / 12.0 * radial_bright;    

}
