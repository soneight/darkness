#ifndef SON8_DARKNESS_NOTDEFINED

#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <utility>

namespace son8::darkness {
    template< typename T >
    concept TestConcept = requires(T t) {
        { t.test() } -> std::same_as< void >;
    };

    template< typename T > struct unref_ { using type = T; };
    template< typename T > struct unref_< T& > { using type = T; };
    template< typename T > struct unref_< T&& > { using type = T; };
    template< typename T > using unref_t = typename unref_< T >::type;

    template< typename T >
    constexpr unref_t< T >&& unref(T&& t) noexcept
    {
        return static_cast< unref_t< T >&& >(t);
    }

    enum class EnumTest: int {
        EnumTest0 = 0, EnumTest1, EnumTest2
    };

    // perfect forwarding
    template< typename T, typename ...Args >
    T make(Args&&... args)
    {
        return T(std::forward< Args >(args)...);
    }

    class UnrefTest {
        using Self = UnrefTest;
        int c_;
        int m_;
    public:
        UnrefTest() : c_(0), m_(static_cast< int >(EnumTest::EnumTest0)) {}
        UnrefTest(int c, int m) : c_(c), m_(m) { }
        ~UnrefTest() {}
        UnrefTest(Self const& s) : c_(++s), m_(s.m_) { }
        UnrefTest(Self&& s) : m_(++s), c_(s.c_) { }
        Self& operator=(Self const& s) { c_ = ++s; m_ = s.m_; return *this; }
        Self& operator=(Self &&s) noexcept { m_ = ++s; c_ = s.c_; return *this; }
        friend int operator++(Self const &s) { return s.c_ + 1; }
        friend int operator++(Self &&s) noexcept { return s.m_ + 1; }

        friend std::ostream& operator<<(std::ostream &os, Self const &s) {
            os << s.c_ << " " << s.m_;
            return os;
        }

        static void test() { std::cout << "UnrefTest::test()" << std::endl; }

        int copies() const { return c_; }
        int moves() const { return m_; }
    };

    class Abstract {
        int a;
    public:
        Abstract() {}
        virtual ~Abstract() {}
        virtual void test() const = 0;
    };

    class Base: public Abstract {
    public:
        void test() const override { std::cout << "Base::test()" << std::endl; }
    };

    class Derived final: public Base {
        void test() const override { std::cout << "Derived::test()" << std::endl; }
    };

} // namespace son8::darkness

int main(int arg, char *argv[]) {
    namespace app = son8::darkness;
    std::cout << "example.cxx" << std::endl;
    app::UnrefTest::test();
    app::UnrefTest ut;
    app::UnrefTest ut2 = std::move(ut);
    app::UnrefTest ut3 = std::move(ut2);
    std::cout << "ut: " << ut3 << std::endl;

    std::vector< app::Abstract* > vec;
    vec.push_back(new app::Base);
    vec.push_back(new app::Derived);
    for (auto &v : vec) {
        v->test();
        delete v;
    }

    auto ut4 = app::make< app::UnrefTest >(8, 6);
    std::cout << "ut4: " << ut4 << std::endl;

    return 0;
} // main

#else // copy from the top to test theme tokenColors when not defined

#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <utility>

namespace son8::darkness {
    template< typename T >
    concept TestConcept = requires(T t) {
        { t.test() } -> std::same_as< void >;
    };

    template< typename T > struct unref_ { using type = T; };
    template< typename T > struct unref_< T& > { using type = T; };
    template< typename T > struct unref_< T&& > { using type = T; };
    template< typename T > using unref_t = typename unref_< T >::type;

    template< typename T >
    constexpr unref_t< T >&& unref(T&& t) noexcept
    {
        return static_cast< unref_t< T >&& >(t);
    }

    enum class EnumTest: int {
        EnumTest0 = 0, EnumTest1, EnumTest2
    };

    // perfect forwarding
    template< typename T, typename ...Args >
    T make(Args&&... args)
    {
        return T(std::forward< Args >(args)...);
    }

    class UnrefTest {
        using Self = UnrefTest;
        int c_;
        int m_;
    public:
        UnrefTest() : c_(0), m_(static_cast< int >(EnumTest::EnumTest0)) {}
        UnrefTest(int c, int m) : c_(c), m_(m) { }
        ~UnrefTest() {}
        UnrefTest(Self const& s) : c_(++s), m_(s.m_) { }
        UnrefTest(Self&& s) : m_(++s), c_(s.c_) { }
        Self& operator=(Self const& s) { c_ = ++s; m_ = s.m_; return *this; }
        Self& operator=(Self &&s) noexcept { m_ = ++s; c_ = s.c_; return *this; }
        friend int operator++(Self const &s) { return s.c_ + 1; }
        friend int operator++(Self &&s) noexcept { return s.m_ + 1; }

        friend std::ostream& operator<<(std::ostream &os, Self const &s) {
            os << s.c_ << " " << s.m_;
            return os;
        }

        static void test() { std::cout << "UnrefTest::test()" << std::endl; }

        int copies() const { return c_; }
        int moves() const { return m_; }
    };

    class Abstract {
        int a;
    public:
        Abstract() {}
        virtual ~Abstract() {}
        virtual void test() const = 0;
    };

    class Base: public Abstract {
    public:
        void test() const override { std::cout << "Base::test()" << std::endl; }
    };

    class Derived final: public Base {
        void test() const override { std::cout << "Derived::test()" << std::endl; }
    };

} // namespace son8::darkness

int main(int arg, char *argv[]) {
    namespace app = son8::darkness;
    std::cout << "example.cxx" << std::endl;
    app::UnrefTest::test();
    app::UnrefTest ut;
    app::UnrefTest ut2 = std::move(ut);
    app::UnrefTest ut3 = std::move(ut2);
    std::cout << "ut: " << ut3 << std::endl;

    std::vector< app::Abstract* > vec;
    vec.push_back(new app::Base);
    vec.push_back(new app::Derived);
    for (auto &v : vec) {
        v->test();
        delete v;
    }

    auto ut4 = app::make< app::UnrefTest >(8, 6);
    std::cout << "ut4: " << ut4 << std::endl;

    return 0;
} // main

#endif
