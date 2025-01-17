#ifdef GL_ES
varying mediump vec4 v_color;
varying mediump vec2 v_texcoord;
#else
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main()
{
#if defined GL_OES_standard_derivatives
    //gl_FragColor = v_color*smoothstep(0.0, length(fwidth(v_texcoord)), 1.0 - length(v_texcoord));
    gl_FragColor = vec4(v_texcoord.x,v_texcoord.y,0,1);
#else
    gl_FragColor = v_color*step(0.0, 1.0 - length(v_texcoord));
#endif
}

