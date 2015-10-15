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

//uniform vec3 lightPos;
uniform float lcutoff;

//uniform vec3 mambient;  // gl_FrontMaterial.ambient
//uniform vec3 mdiffuse;  // gl_FrontMaterial.diffuse
//uniform vec3 mspecular; // gl_FrontMaterial.specular
//uniform float shininess;// gl_FrontMaterial.shininess

//uniform vec3 lambient;  // gl_LightSource[0].ambient
//uniform vec3 ldiffuse;  // gl_LightSource[?]
//uniform vec3 lspecular;  // gl_LightSource[?]

uniform vec3 spotDirection;
uniform vec3 dirLightDir;

void main() {

	//gl_FrontMaterial.ambient = texture2D(texture0, vTextureCoord0);
	//gl_FrontMaterial.diffuse = texture2D(texture0, vTextureCoord0);
	//gl_FrontMaterial.specular = texture2D(texture0, vTextureCoord0);
	vec4 mater = texture2D(texture0, vTextureCoord0);
	//mater.xyz

	// AMBIENT
	//vec3 ambient = gl_FrontMaterial.ambient.xyz * gl_LightSource[3].ambient.xyz;
	vec3 ambient = gl_LightSource[3].ambient.xyz;

	// DIFFUSE

		// POINT
		float dist = length( vPosition - gl_LightSource[1].position.xyz);
		float attenuation = 1.0 / (0.2 + 0.1 * dist + 0.1 * dist * dist);		
		vec3 surf2light = normalize(gl_LightSource[1].position.xyz - vPosition);
		vec3 norm = normalize(vNormal);
		float dcont = max(dot(norm, surf2light), 0.0);
		//vec3 diffuse = dcont * mater.xyz * gl_LightSource[1].diffuse.xyz;
		vec3 diffuse = dcont * gl_LightSource[1].diffuse.xyz;

		// SPOT
		dist = length( vPosition - gl_LightSource[0].position.xyz);
		attenuation = 1.0 / (0.2 + 0.1 * dist + 0.1 * dist * dist);		
		
		surf2light = normalize(gl_LightSource[0].position.xyz - vPosition);
		vec3 spotDir = normalize(gl_LightSource[0].spotDirection.xyz );
		spotDir = -spotDir;
		float dotSurfAndDirec = degrees( acos( dot(surf2light,spotDir) ) );
		if (dotSurfAndDirec < gl_LightSource[0].spotCutoff){ 
					dcont = max(dot(norm, surf2light), 0.0);
					//diffuse += dcont * mater.xyz * gl_LightSource[0].diffuse.xyz;
					diffuse += dcont * gl_LightSource[0].diffuse.xyz;
		}
		else if (dotSurfAndDirec < gl_LightSource[0].spotCutoff + 2){ 
					dcont = max(dot(norm, surf2light), 0.0) *  (1-((dotSurfAndDirec - gl_LightSource[0].spotCutoff) / 2));
					//diffuse += dcont * mater.xyz * gl_LightSource[0].diffuse.xyz;
					diffuse += dcont * gl_LightSource[0].diffuse.xyz;
		}

		// DIRECTION
		vec3 dirLight = normalize ( gl_LightSource[2].position.xyz );
		dcont = max(dot(norm, dirLight), 0.0);
		//diffuse += dcont * gl_FrontMaterial.diffuse.xyz * gl_LightSource[2].diffuse.xyz;
		diffuse += dcont * gl_LightSource[2].diffuse.xyz;

	// SPECULAR

		// POINT
		vec3 surf2view = normalize ( -vPosition );
		surf2light = normalize(gl_LightSource[1].position.xyz - vPosition);
		vec3 reflection = reflect(-surf2light, norm);
		float scont = pow( max( dot(surf2view, reflection), 0.0), gl_FrontMaterial.shininess);
		//vec3 specular = scont * gl_LightSource[1].specular.xyz * gl_FrontMaterial.specular.xyz;
		vec3 specular = scont * gl_LightSource[1].specular.xyz;

		// SPOT
		if (dotSurfAndDirec < gl_LightSource[0].spotCutoff){ 
					surf2light = normalize(gl_LightSource[0].position.xyz - vPosition);
					reflection = reflect(-surf2light, norm);
					scont = pow( max( dot(surf2view, reflection), 0.0), gl_FrontMaterial.shininess);
					//specular += scont * gl_LightSource[0].specular.xyz * gl_FrontMaterial.specular.xyz;
					specular += scont * gl_LightSource[0].specular.xyz;
		}
		else if (dotSurfAndDirec < gl_LightSource[0].spotCutoff + 2){ 
					surf2light = normalize(gl_LightSource[0].position.xyz - vPosition);
					reflection = reflect(-surf2light, norm);
					scont = pow( max( dot(surf2view, reflection), 0.0), gl_FrontMaterial.shininess);
					//specular += scont * gl_LightSource[0].specular.xyz * gl_FrontMaterial.specular.xyz;
					specular += scont * gl_LightSource[0].specular.xyz * (1-((dotSurfAndDirec - gl_LightSource[0].spotCutoff) / 2));
		}

		// DIRECTION
		surf2light = normalize(dirLightDir - vPosition);
		surf2light = -surf2light;
		reflection = reflect(-surf2light, norm);
		scont = pow( max( dot(surf2view, reflection), 0.0), gl_FrontMaterial.shininess);
		//specular += scont * gl_LightSource[2].specular.xyz * gl_FrontMaterial.specular.xyz;
		specular += scont * gl_LightSource[2].specular.xyz;

	vec4 bla = texture2D(texture0, vTextureCoord0);
	//gl_FragColor = normalize (vec4(ambient+diffuse+specular, 1.0) + bla );
	gl_FragColor = vec4(ambient+diffuse+specular, 1.0) * bla ;
	//gl_FragColor = vec4(ambient+diffuse+specular, 1.0);
}