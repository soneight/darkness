const canvas = document.getElementById( 'canvas-gl' );
const gl = canvas.getContext( "webgl" )

if ( !gl ) console.log( 'WebGL context is not supported' );
else {
    gl.clearColor( 0.1, 0.4, 0.4, 0.0 );
    gl.clear( gl.COLOR_BUFFER_BIT );
}
