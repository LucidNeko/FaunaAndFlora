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

// varying vec4 lightPos;
const int radial_samples = 64;
const float radial_blur = 0.8;   // blur factor
const float radial_exposure = 0.90; // bright factor

void main() {

  vec2 TexCoord = vTextureCoord0 - vLight.xy;
  vec4 SumColor= vec4(0.0, 0.0, 0.0, 0.0);


  /*
  // BOX LIGHT
  if (vTextureCoord0.y < vLight.y){ // We are under the light source
    if (abs(vTextureCoord0.x - vLight.x) < 0.2){ // We are within the bounds
      // float x = 1.0;
      for (int i = 0; i < radial_samples; i++) 
      {
        // float scale = 1.0 - radial_blur * (float(i) / float(radial_samples));
        // SumColor += texture2D(texture0, TexCoord * scale + vLight.xy);
        float scale = 1.0 - radial_blur * (float(i) / float(radial_samples));
        SumColor += texture2D(texture0, vec2(vTextureCoord0.x, TexCoord.y * scale + vLight.y));
      }
    }
  }
  */

  /*
  // SPOT LIGHT
  vec2 lightDirection = normalize(vec2(-1.0,-1.0)); // Light pointing down and left
  vec2 pixelToLight = normalize(vec2(vTextureCoord0 - vLight.xy));
  float dotPixToLight = degrees(acos(dot(pixelToLight, lightDirection)));
  float theta = 36.5;
  if (dotPixToLight < theta){ // We are under the light source
    // if (abs(vTextureCoord0.x - vLight.x) < 0.2){ // We are within the bounds
      // float x = 1.0;
      for (int i = 0; i < radial_samples; i++) 
      {
        float scale = 1.0 - radial_blur * (float(i) / float(radial_samples));
        SumColor += texture2D(texture0, TexCoord * scale + vLight.xy);
        // float scale = 1.0 - radial_blur * (float(i) / float(radial_samples));
        // SumColor += texture2D(texture0, vec2(vTextureCoord0.x, TexCoord.y * scale + vLight.y));
      }
    // }
  }
  */
  
  
  // POINT LIGHT
  for (int i = 0; i < radial_samples; i++) 
  {
    float scale = 1.0 - radial_blur * (float(i) / float(radial_samples));
    // SumColor += texture2D(texture0, TexCoord * scale + vLight.xy);
  }
  


  gl_FragColor = SumColor / float(radial_samples) * radial_exposure;    
}
