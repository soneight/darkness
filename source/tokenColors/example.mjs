// nodejs server example, that is part of web examples overall
import { createServer } from 'node:http';
import { fileURLToPath } from 'node:url';
import { readFile } from 'node:fs/promises';
import path from 'node:path';

const App = {
    Port: 3000,
    Host: '127.0.0.1',
    Public_Dir: path.dirname( fileURLToPath( import.meta.url ) ),
    Valid_Pages: ['example.html', 'example.css', 'example.js'],
    Mime: {
        '.html': 'text/html',
        '.css': 'text/css',
        '.js': 'text/javascript',
    },
    validate( request ) {
        const q = request;
        const method = q.method || '';
        if ( method !== 'GET' ) throw 'only get methods allowed';
        const page = q.url == '/' ? 'example.html' : q.url.substring( 1 );
        const found = this.Valid_Pages.includes( page );
        if ( !found ) throw 'page: ' + page + ' not found';
        console.log( q.method + ': ' + q.url );
        return page;
    },
    async send_response( request, response ) {
        const s = response;
        try {
            const page = this.validate( request );
            const text = await readFile( path.join( this.Public_Dir, page ) );
            s.writeHead( 200, { 'Content-Type': this.Mime[path.extname( page )] } );
            s.end( text, 'utf-8' );
        } catch ( error ) {
            console.log( error );
            s.writeHead( 404 );
            s.end( 'Not Found', 'utf-8' );
        };
    },
};

const example = createServer( async ( q, s ) => { await App.send_response( q, s ); } );

example.listen( App.Port, App.Host, ( ) => {
    console.log( 'Public Dir: ' + App.Public_Dir );
    console.log( `listen on http://${App.Host}:${App.Port}` );
});
