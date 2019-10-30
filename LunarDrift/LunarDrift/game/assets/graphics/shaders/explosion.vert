#version 400

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

uniform vec4 u_StartColor;
uniform vec4 u_Decay;
uniform float u_DecayTime;
uniform float u_Timestamp;

in vec3 in_Position;
in vec3 in_Direction;
in float in_BirthStamp;
in float in_Speed;
in float in_Life;
in float in_DecayRate;

out vec4 out_Color;

void main()
{
	if (in_BirthStamp < 0.f)
	{
		out_Color = vec4(0.0, 0.0, 0.0, 0.0);
		gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
		return;
	}

	float deltaTime = u_Timestamp - in_BirthStamp;

	float decayMagnitude = min(deltaTime, u_DecayTime) * in_DecayRate;
	vec4 color = u_StartColor - (u_Decay * decayMagnitude);
	
	vec3 pos = in_Position * in_Direction * in_Speed * deltaTime;

	out_Color = color;
	gl_Position = u_ProjMatrix * u_ViewMatrix * vec4(pos, 1.0);
}
