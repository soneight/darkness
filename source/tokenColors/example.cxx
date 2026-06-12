#ifndef SON8_DARKNESS_NOTDEFINED
// header
#ifndef HEADER_HXX
#define HEADER_HXX

#include <GL/gl.h>
#include <GLFW/glfw3.h>

namespace son8::darkness {
    template< typename Type_ > using Ptr = Type_ *;
    template< typename Type_ > using Out = Type_ &;
    template< typename Type_ > using Uni = Type_ &&;
    template< typename Type_ > using Ref = Type_ const &;
    using Bool = bool;
    using Flag = unsigned;
    using Void = void;
} // son8::darkness

#endif//HEADER_HXX
// source
#include <ctime>
#include <exception>
#include <iostream>

namespace app {
    using namespace son8::darkness;

    class Exception : public std::exception {
        using Message_ = Ptr< char const >;
        Message_ msg_;
    public:
        using ValueType = Message_;
        using Ref = app::Ref< Exception >;
        Exception( Ptr< char const > msg ) : msg_{ msg } { }
        auto what( ) const noexcept -> ValueType { return msg_; }
    };

    class InitGLFW final {
    public:
        InitGLFW( ) { if ( not glfwInit( ) ) throw Exception{ "glfwInit failed" }; }
       ~InitGLFW( ) { glfwTerminate( ); }
    };

    class WindowGLFW final {
        GLFWwindow *window_;
    public:
        static constexpr auto Size_X = 640u;
        static constexpr auto Size_Y = 360u;
        WindowGLFW( ) {
            glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
            window_ = glfwCreateWindow( Size_X, Size_Y, "GLFW Window", nullptr, nullptr );
            if ( not window_ ) throw Exception{ "glfwCreateWindow failed" };
            activate_context( );
            glfwSwapInterval( 0 );
        }
       ~WindowGLFW( ) { glfwDestroyWindow( window_ ); }
        operator GLFWwindow *( ) { return window_; }
        Void activate_context( ) { glfwMakeContextCurrent( window_ ); }
        Bool is_running( ) const { return not glfwWindowShouldClose( window_ ); }
        Void swap_buffers( ) const { glfwSwapBuffers( window_); }
    };

    Void draw_gl( );
}
auto main( [[maybe_unused]] int argc, [[maybe_unused]] char *argv[] ) -> int try {
    using Window = app::WindowGLFW;
    app::InitGLFW init;
    app::WindowGLFW window;
    auto currFrame = 0u;
    auto prevFrame = currFrame;

    glViewport( 0, 0, Window::Size_X, Window::Size_Y );

    while ( window.is_running( ) ) {
        auto timeBeg = std::time( nullptr );
        // first is events
        glfwPollEvents( );
        // middle is draw
        app::draw_gl( );
        // last is swap
        window.swap_buffers( );

        auto timeEnd = std::time( nullptr );

        auto timeDiff = (intmax_t)timeEnd - (intmax_t)timeBeg;
        if ( timeDiff ) {
            std::cout << "fps (TODO): " << ( currFrame - prevFrame ) / timeDiff << std::endl;
            prevFrame = currFrame;
        }
        ++currFrame;
    }

} catch ( app::Exception::Ref e ) {
    std::cerr << "std::exception: " << e.what( ) << std::endl;
} catch ( ... ) {
    std::cerr << "... unhandled exception" << std::endl;
}
// definitions
namespace app {
    Void draw_gl( ) {
        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor( .1f, .4f, .4f, 1.f );
        glFlush( );
    }
}
// check hidden preprocessor (must be less bright)
#else //SON8_DARKNESS_NOTDEFINED
// header
#ifndef HEADER_HXX
#define HEADER_HXX
#include <utility>

namespace son8::darkness {
    template< typename Type_ > using Ptr = Type_ *;
    template< typename Type_ > using Out = Type_ &;
    template< typename Type_ > using Uni = Type_ &&;
    template< typename Type_ > using Ref = Type_ const &;

    template< typename Type_ >
    void swap( Out< Type_ > lt, Out< Type_ > rt ) {
        Uni< Type_ > t_ = std::move( lt );
        lt = std::move( rt );
        rt = std::move( t_ );
    }
} // son8::darkness

#endif//HEADER_HXX
// source
#include <exception>
#include <iostream>
namespace app {
    using namespace son8::darkness;

    class Exception : public std::exception {
        using Message_ = Ptr< char const >;
        Message_ msg_;
    public:
        using ValueType = Message_;
        using Ref = app::Ref< Exception >;
        Exception( Ptr< char const > msg ) : msg_{ msg } { }
        auto what( ) const noexcept -> ValueType { return msg_; }
    };
}
int main( int argc, char *argv[] ) try {
    if ( argc != 2 ) throw app::Exception{ "app expects exactly one argument" };
    app::swap( argv[0], argv[1] );
    std::cout << argv[0] << std::endl;
} catch ( app::Exception::Ref e ) {
    std::cerr << "std::exception: " << e.what( ) << std::endl;
} catch ( ... ) {
    std::cerr << "... unhandled exception" << std::endl;
}
#endif//SON8_DARKNESS_NOTDEFINED
