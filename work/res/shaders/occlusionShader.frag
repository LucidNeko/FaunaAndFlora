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

uniform int isLight;

void main() {
	gl_FragColor = isLight==1 ? vec4(0.5,0.5,0.5,1) : vec4(0,0,0,1);		
	// gl_FragColor = isLight==1 ? vec4(1,1,1,1) : vec4(0.1,0.67,0.19,0.2);	
	if (isLight==3) gl_FragColor = vec4(0.8,0.67,0.19,0.2);
}