#ifndef SON8_DARKNESS_NOTDEFINED
// header
#ifndef HEADER_HXX
#define HEADER_HXX

#include <GLFW/glfw3.h>

namespace son8::darkness {
    template< typename Type_ > using Ptr = Type_ *;
    template< typename Type_ > using Out = Type_ &;
    template< typename Type_ > using Uni = Type_ &&;
    template< typename Type_ > using Ref = Type_ const &;
    using Bool = bool;
} // son8::darkness

#endif//HEADER_HXX
// source
// TODO: add graphics using glfw with modern OpenGL
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
        WindowGLFW( ) {
            window_ = glfwCreateWindow( 640, 360, "GLFW Window", nullptr, nullptr );
            if ( not window_ ) throw Exception{ "glfwCreateWindow failed" };
        }
       ~WindowGLFW( ) { }
        operator GLFWwindow *( ) { return window_; }
        Bool is_running( ) { return not glfwWindowShouldClose( window_ ); }
    };
}
int main( [[maybe_unused]] int argc, [[maybe_unused]] char *argv[] ) try {
    app::InitGLFW init;
    app::WindowGLFW window;

    while ( window.is_running( ) ) {
        glfwPollEvents( );
    }

} catch ( app::Exception::Ref e ) {
    std::cerr << "std::exception: " << e.what( ) << std::endl;
} catch ( ... ) {
    std::cerr << "... unhandled exception" << std::endl;
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
