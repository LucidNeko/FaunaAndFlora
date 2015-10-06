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

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying float vDepth;

void main() {
	gl_FragData[0] = vec4(vPosition, 1.0);
	gl_FragData[1] = vec4(vNormal, 1.0);
	gl_FragData[2] = vec4(vDepth, vDepth, vDepth, 1.0);
}