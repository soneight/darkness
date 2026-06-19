const canvas = document.getElementById( 'canvas-gl' );
const gl = canvas.getContext( "webgl" )

let isClosing = false;
let isInside = false;
let currFrame = 0, prevFrame = 0;
let currTime, prevTime;
let prevSizeX, prevSizeY;
let currSizeX, currSizeY;
let currFPS = 0, prevFPS = 0;

function is_curr_size_changed_and_updated( ) {
    currSizeX = Math.floor( canvas.clientWidth * window.devicePixelRatio );
    currSizeY = Math.floor( canvas.clientHeight * window.devicePixelRatio );
    if ( prevSizeX === currSizeX && prevSizeY == currSizeY ) return false;
    prevSizeX = currSizeX;
    prevSizeY = currSizeY;
    return true;
}

canvas.addEventListener( 'mouseenter', ( ev ) => { isInside = true } );
canvas.addEventListener( 'mouseleave', ( ev ) => { isInside = false; actionQueue.length = 0 } );

let actionQueue = [];

const Actions = {
    close: 1
};

window.addEventListener( 'keydown', ( ev ) => {
    if ( !isInside ) return;

    let action;
    if ( ev.ctrlKey && ev.key === 'Escape' ) action = Actions.close;

    if ( !action ) return;
    ev.preventDefault( );
    actionQueue.push( action );
});

function poll( ) {
    for ( let i = 0; i < actionQueue.length; ++i ) {
        const action = actionQueue[i];
        switch ( action ) {
        case Actions.close: isClosing = true; break;
            default: continue;
        }
    }
    actionQueue.length = 0;
}

function sync( ) {
    if ( is_curr_size_changed_and_updated( ) ) {
        console.log( "size updated" )
        canvas.width = currSizeX;
        canvas.height = currSizeY;
        gl.viewport( 0, 0, canvas.width, canvas.height );
    }
}

function draw( ) {
    gl.clearColor( 0.1, 0.4, 0.4, 1.0 );
    gl.clear( gl.COLOR_BUFFER_BIT );
}

function show( ) {
    requestAnimationFrame( render );
}

function render( ) {
    if ( isClosing ) {
        const ext = gl.getExtension('WEBGL_lose_context');
        if ( ext ) {
            console.log( 'lose context extension found!' );
            ext.loseContext();
        }
        canvas.remove( );
        return;
    }
    poll( );
    sync( );
    draw( );
    show( );

    currTime = performance.now( );
    if ( currTime - prevTime >= 1000 ) {
        currFPS = currFrame - prevFrame;
        if ( Math.abs( currFPS - prevFPS ) ) console.log( `FPS: ${currFPS}` );
        prevFPS = currFPS;
        prevFrame = currFrame;
        prevTime = currTime;
    }
    ++currFrame;
}

if ( !gl ) console.log( 'WebGL context is not supported' );
else {
    prevTime = performance.now( );
    render( );
}
