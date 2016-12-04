attribute vec4 a_position;
attribute vec2 a_texCoord;
uniform vec4 u_color;
uniform float u_pointSize;
uniform vec2 u_texSize;
#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying lowp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif
uniform float u_ratio;
void main()
{
	vec4 pos = vec4(a_position.xy * u_ratio , 1,1);
	gl_Position = CC_PMatrix * pos;
	gl_PointSize= u_pointSize;
	v_fragmentColor = u_color;
	v_texCoord = a_texCoord;
}