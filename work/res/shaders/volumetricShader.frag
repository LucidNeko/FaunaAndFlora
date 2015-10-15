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
// uniform vec3 lPos;
// uniform vec3 l_Pos;


// const float sampleDist = 1.0;
// const float sampleStrength = 2.2; 

// #define T texture2D(texture0,.5+(p.xy*=.992))

// uniform sampler2D Texture;   // scene texture
// uniform vec2 radial_size;    // texel size
 // uniform float radial_blur;   // blur factor
// uniform float radial_bright; // bright factor
 // uniform vec2 radial_origin;  // blur origin

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
/*
vec3 resolution = vec3(640,480,100);

  vec3 p = gl_FragCoord.xyz/resolution-.5;
  vec3 o = T.rbb;
  for (float i=0.;i<100.;i++) 
    p.z += pow(max(0.,.5-length(T.rg)),2.)*exp(-i*.08);
  gl_FragColor=vec4(o*o+p.z,1);
*/
    /*
	// gl_FragColor = texture2D(texture0, vTextureCoord0);
	// gl_FragColor = vec4(1,0,0,1);
    // vec4 Pos = gl_ModelViewMatrixInverse * vec4(vLight, 1.0);

	vec4 Color = vec4(0,0,0,0);
    float v = 0.0;

    // normalize(lightPos);
    // float midx = 0.5;//lightPos.x;
    // float midy = 0.5;//lightPos.y;
    float midx = lightPos.x;
    float midy = lightPos.y;
    // mid = 0.0;
    for( float i=0.0;i<1.0;i+=1.0/float(Quality) )
    {
            v = 0.9+i*0.1;//convert "i" to the 0.9 to 1 range
            float uvU = (vTextureCoord0*v+midx-midx*v).x;
            float uvV = (vTextureCoord0*v+midy-midy*v).y;
            // if (uvU < 0.0) {uvU == 0.0;}
            // if (uvU > 1.0) {uvU == 1.0;}
            // if (uvV < 0.01) {uvV == 0.01;}
            // if (uvV > 0.99) {uvV == 0.99;}
            vec2 texCoo = vec2(uvU, uvV);
            Color += texture2D( texture0, texCoo);
    }
    Color /= float(Quality);
    gl_FragColor =  Color;// *  v_vColour;
    // gl_FragColor =  Pos;// *  v_vColour;
    */


/*
    float samples[10];
    samples[0] = -0.08;
    samples[1] = -0.05;
    samples[2] = -0.03;
    samples[3] = -0.02;
    samples[4] = -0.01;
    samples[5] =  0.01;
    samples[6] =  0.02;
    samples[7] =  0.03;
    samples[8] =  0.05;
    samples[9] =  0.08;

    vec2 dir = lightPos.xy - vTextureCoord0; 
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
    dir = dir/dist; 

    vec4 color = texture2D(texture0,vTextureCoord0); 
    vec4 sum = color;

    for (int i = 0; i < 10; i++)
        sum += texture2D( texture0, vTextureCoord0 + dir * samples[i] * sampleDist );

    sum *= 1.0/11.0;
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0);

    gl_FragColor = mix( color, sum, t );
    */
}
