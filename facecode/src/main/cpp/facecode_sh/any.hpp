
#pragma once

#ifndef z3a1e8a65e1
#define z3a1e8a65e1

#define zd5e829be5b  (0x16d9+278-0x17ef)
#define z5fd767c729  (0xeb4+1725-0x156f)
#define zd0983d57b5  (0x205b+915-0x23ee)

#define z19ead7e725  z20013cc20b(zd5e829be5b) "\x2e" z20013cc20b(z5fd767c729) "\x2e" z20013cc20b(zd0983d57b5)

#define z20013cc20b(  x )  any_STRINGIFY_( x )
#define any_STRINGIFY_( x )  #x


#define ze93cdc1246  (0xaff+3733-0x1994)
#define zd87125394b   (0xded+483-0xfcf)
#define zb0cbaead7b      (0x21ad+148-0x223f)

#if !defined( zfd8a8facc9 )
# define zfd8a8facc9  ( any_HAVE_STD_ANY ? zb0cbaead7b : zd87125394b )
#endif


#ifndef z0d087dffd2
# if defined(z72999b4584) || defined(z8a84fadcce) || defined(_CPPUNWIND)
#  define z0d087dffd2  (0x1e4+1258-0x6ce)
# else
#  define z0d087dffd2  (0x138+5162-0x1561)
# endif
#endif


#ifndef   any_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(z698f1cdbc8)
#  define any_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define any_CPLUSPLUS  __cplusplus
# endif
#endif

#define za5e3d58b31  ( any_CPLUSPLUS >= 199711L )
#define z97003e353b  ( any_CPLUSPLUS >= 201103L )
#define z42c0c4f958  ( any_CPLUSPLUS >= 201402L )
#define zb67155cf9e  ( any_CPLUSPLUS >= 201703L )
#define z6e8147a0e7  ( any_CPLUSPLUS >= 202000L )


#if zb67155cf9e && defined(__has_include )
# if __has_include( <any> )
#  define any_HAVE_STD_ANY  (0x13f3+2338-0x1d14)
# else
#  define any_HAVE_STD_ANY  (0x3fb+1811-0xb0e)
# endif
#else
# define  any_HAVE_STD_ANY  (0xd+1753-0x6e6)
#endif

#define  z84cc9b4930  ( (zfd8a8facc9 == zb0cbaead7b) || ((zfd8a8facc9 == ze93cdc1246) && any_HAVE_STD_ANY) )


#ifndef z56bd711434
#define z56bd711434  (0x862+4019-0x1814)


#if zb67155cf9e

#include <utility>

namespace fc {

using std::in_place;
using std::in_place_type;
using std::in_place_index;
using std::in_place_t;
using std::in_place_type_t;
using std::in_place_index_t;

#define zc5fe0c51bf(      T)  std::in_place_t
#define nonstd_lite_in_place_type_t( T)  std::in_place_type_t<T>
#define z4d3dd4f214(K)  std::in_place_index_t<K>

#define zc009ac7023(      T)    std::in_place_t{}
#define za62e91c7de( T)    std::in_place_type_t<T>{}
#define z7dcdb62e41(K)    std::in_place_index_t<K>{}

} 
#else 

#include <cstddef>




namespace fc {
    namespace zf3831dc9b1 {

        template< class T >
        struct in_place_type_tag {};

        template< std::size_t K >
        struct in_place_index_tag {};

    } 
    struct in_place_t {};

    template< class T >
    inline in_place_t in_place( zf3831dc9b1::in_place_type_tag<T> = zf3831dc9b1::in_place_type_tag<T>() )
    {
        return in_place_t();
    }

    template< std::size_t K >
    inline in_place_t in_place( zf3831dc9b1::in_place_index_tag<K> = zf3831dc9b1::in_place_index_tag<K>() )
    {
        return in_place_t();
    }

    template< class T >
    inline in_place_t in_place_type( zf3831dc9b1::in_place_type_tag<T> = zf3831dc9b1::in_place_type_tag<T>() )
    {
        return in_place_t();
    }

    template< std::size_t K >
    inline in_place_t in_place_index( zf3831dc9b1::in_place_index_tag<K> = zf3831dc9b1::in_place_index_tag<K>() )
    {
        return in_place_t();
    }

#define zc5fe0c51bf(      T)  fc::in_place_t(&)( fc::zf3831dc9b1::in_place_type_tag<T>  )
#define nonstd_lite_in_place_type_t( T)  fc::in_place_t(&)( fc::zf3831dc9b1::in_place_type_tag<T>  )
#define z4d3dd4f214(K)  fc::in_place_t(&)( fc::zf3831dc9b1::in_place_index_tag<K> )

#define zc009ac7023(      T)    fc::in_place_type<T>
#define za62e91c7de( T)    fc::in_place_type<T>
#define z7dcdb62e41(K)    fc::in_place_index<K>

} 


#endif 
#endif 






#if z84cc9b4930

#include <any>
#include <utility>

namespace fc {

    using std::any;
    using std::any_cast;
    using std::make_any;
    using std::swap;
    using std::bad_any_cast;
}

#else 

#include <utility>


#if defined(_MSC_VER ) && !defined(z698f1cdbc8)
# define z1a90d8c4d9      (_MSC_VER )
# define z0426bea148  (_MSC_VER / (0x11f1+1918-0x1965) - (0x446+7507-0x218f) * ( (0xad4+5359-0x1fbe) + (_MSC_VER < (0xa13+7315-0x1f3a) ) ) )
#else
# define z1a90d8c4d9      (0x16fa+3110-0x2320)
# define z0426bea148  (0x21b6+853-0x250b)
#endif

#define zbf582910a9( major, minor, patch )  ( (0xea4+3868-0x1db6) * ( (0x93a+4537-0x1ae9) * (major) + (minor) ) + (patch) )

#if defined(z698f1cdbc8)
# define z13834aee81  zbf582910a9(z3784e5bd8c, z9970ecdfbc, zd14a6eea1e)
#else
# define z13834aee81  (0x2a1+2390-0xbf7)
#endif

#if defined(__GNUC__) && !defined(z698f1cdbc8)
# define zacb4bfb134  zbf582910a9(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define zacb4bfb134  (0xaf4+6359-0x23cb)
#endif



#define z852314f63f( z94a2d6b94f )  ( z5bfbeea531##z94a2d6b94f )

#ifdef _HAS_CPP0X
# define z0c2eea80c0  _HAS_CPP0X
#else
# define z0c2eea80c0  (0x17c3+1035-0x1bce)
#endif

#define z6b62ad2133   (z97003e353b || z1a90d8c4d9 >= (0x1268+1514-0x1276))
#define z5dd1899a48  (z97003e353b || z1a90d8c4d9 >= 1600)
#define z003822b978  (z97003e353b || z1a90d8c4d9 >= (0x1bc2+2191-0x1d49))
#define zceffbb7d64  (z97003e353b || z1a90d8c4d9 >= (0x1db1+3419-0x23a0))

#define zc967628a39  (z42c0c4f958)
#define za5b5d738cb  (zb67155cf9e)


#define z3e73a8eab9           zceffbb7d64
#define z2fc313d89d \
                                        z003822b978
#define z85cd3d7d4a       z003822b978
#define z92c1783488               zceffbb7d64
#define z12e0ecbca3                z5dd1899a48
#define z6448d1a3a2            z6b62ad2133
#define zfa6101661f          z5dd1899a48
#define z08db21835e              z6b62ad2133
#define ze005600080       z6b62ad2133

#define zfc468d8f52          (!! zacb4bfb134 )
#define z681848ceac   (!! zacb4bfb134 )
#define zc40a9c4764        (!! zacb4bfb134 )


#define zddbebf6dfd           zc967628a39


#define zcafa3cd7b0              za5b5d738cb


#if z3e73a8eab9
# define z0ce1e89f51 constexpr
#else
# define z0ce1e89f51 
#endif

#if zddbebf6dfd
# define z4f15750d9f constexpr
#else
# define z4f15750d9f 
#endif

#if z92c1783488
# define any_noexcept noexcept
#else
# define any_noexcept 
#endif

#if z12e0ecbca3
# define any_nullptr nullptr
#else
# define any_nullptr NULL
#endif

#if zcafa3cd7b0
# define zcaa661760c [[z5c9ab6580d]]
#else
# define zcaa661760c 
#endif


#if z0d087dffd2
# include <cassert>
#else
# include <typeinfo>
#endif

#if ! z12e0ecbca3
# include <cstddef>
#endif

#if z85cd3d7d4a
# include <initializer_list>
#endif

#if z6448d1a3a2
# include <type_traits>
#elif zc40a9c4764
# include <tr1/type_traits>
#endif


#if z97003e353b

#define zb2ce4c8727(...) \
    template< bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = (0x19a8+2059-0x21b3) >

#define any_REQUIRES_T(...) \
    , typename = typename std::enable_if< (__VA_ARGS__), fc::zf3831dc9b1::z7a61c02b2c >::type

#define zd495828dbd(z7d1ff146f1, ...) \
    typename std::enable_if<__VA_ARGS__, z7d1ff146f1>::type

#define z4f3f73d1ad(...) \
    , typename std::enable_if<__VA_ARGS__, void*>::type = nullptr

#endif


namespace fc {  

        namespace z9c38ccefe1 {

#if z08db21835e

            using std::add_const;

#elif zfc468d8f52

            using std::z617e9bdef7::add_const;

#else

template< class T > struct add_const { typedef const T Type; };

#endif 

#if ze005600080

            using std::remove_reference;

#elif z681848ceac

            using std::z617e9bdef7::remove_reference;

#else

template< class T > struct remove_reference     { typedef T Type; };
template< class T > struct remove_reference<T&> { typedef T Type; };

#endif 

        } 
        namespace zf3831dc9b1 {


 class z7a61c02b2c{};

        } 
#if ! z0d087dffd2

        class bad_any_cast : public std::bad_cast
        {
        public:
#if z97003e353b
            virtual const char* what() const any_noexcept
#else
            virtual const char* what() const throw()
#endif
            {
                return "\x61\x6e\x79\x2d\x6c\x69\x74\x65\x3a\x20\x62\x61\x64\x20\x61\x6e\x79\x5f\x63\x61\x73\x74";
            }
        };

#endif 

        class any
        {
        public:
            z0ce1e89f51 any() any_noexcept
                    : zf940a9d627( any_nullptr )
            {}

            any( any const & other )
                    : zf940a9d627( other.zf940a9d627 ? other.zf940a9d627->clone() : any_nullptr )
            {}

#if z97003e353b

            any( any && other ) any_noexcept
                    : zf940a9d627( std::move( other.zf940a9d627 ) )
            {
                other.zf940a9d627 = any_nullptr;
            }

            template<
                    class z75c98007b4, class T = typename std::decay<z75c98007b4>::type
                    any_REQUIRES_T( ! std::is_same<T, any>::value )
            >
            any( z75c98007b4 && value ) any_noexcept
                    : zf940a9d627( new z008fe3fade<T>( std::move( value ) ) )
            {}

            template<
                    class T, class... z7efd81ba62
                    any_REQUIRES_T( std::is_constructible<T, z7efd81ba62&&...>::value )
            >
            explicit any( nonstd_lite_in_place_type_t(T), z7efd81ba62&&... z86fe05c556 )
                    : zf940a9d627( new z008fe3fade<T>( T( std::forward<z7efd81ba62>(z86fe05c556)... ) ) )
            {}

            template<
                    class T, class z37d26b6206, class... z7efd81ba62
                    any_REQUIRES_T( std::is_constructible<T, std::initializer_list<z37d26b6206>&, z7efd81ba62&&...>::value )
            >
            explicit any( nonstd_lite_in_place_type_t(T), std::initializer_list<z37d26b6206> z46e3444520, z7efd81ba62&&... z86fe05c556 )
                    : zf940a9d627( new z008fe3fade<T>( T( z46e3444520, std::forward<z7efd81ba62>(z86fe05c556)... ) ) )
            {}

#else

            template< class z75c98007b4 >
    any( z75c98007b4 const & value )
    : zf940a9d627( new z008fe3fade<z75c98007b4>( value ) )
    {}

#endif 

            ~any()
            {
                reset();
            }

            any & operator=( any const & other )
            {
                any( other ).swap( *this );
                return *this;
            }

#if z97003e353b

            any & operator=( any && other ) any_noexcept
            {
                any( std::move( other ) ).swap( *this );
                return *this;
            }

            template<
                    class z75c98007b4, class T = typename std::decay<z75c98007b4>::type
                    any_REQUIRES_T( ! std::is_same<T, any>::value )
            >
            any & operator=( z75c98007b4 && value )
            {
                any( std::move( value ) ).swap( *this );
                return *this;
            }

            template< class T, class... z7efd81ba62 >
            void emplace( z7efd81ba62 && ... z86fe05c556 )
            {
                any( T( std::forward<z7efd81ba62>(z86fe05c556)... ) ).swap( *this );
            }

            template<
                    class T, class z37d26b6206, class... z7efd81ba62
                    any_REQUIRES_T( std::is_constructible<T, std::initializer_list<z37d26b6206>&, z7efd81ba62&&...>::value )
            >
            void emplace( std::initializer_list<z37d26b6206> z46e3444520, z7efd81ba62&&... z86fe05c556 )
            {
                any( T( z46e3444520, std::forward<z7efd81ba62>(z86fe05c556)... ) ).swap( *this );
            }

#else

            template< class z75c98007b4 >
    any & operator=( z75c98007b4 const & value )
    {
        any( value ).swap( *this );
        return *this;
    }

#endif 

            void reset() any_noexcept
            {
                delete zf940a9d627; zf940a9d627 = any_nullptr;
            }

            void swap( any & other ) any_noexcept
            {
                std::swap( zf940a9d627, other.zf940a9d627 );
            }

            bool z45276b2cad() const any_noexcept
            {
                return zf940a9d627 != any_nullptr;
            }

            const std::type_info & type() const any_noexcept
            {
                return z45276b2cad() ? zf940a9d627->type() : typeid( void );
            }

                                    
            template< class z75c98007b4 >
            const z75c98007b4 * z9ff1d873cd() const
            {
                return &( static_cast<z008fe3fade<z75c98007b4> *>( zf940a9d627 )->zb81bbfc639 );
            }

            template< class z75c98007b4 >
            z75c98007b4 * z9ff1d873cd()
            {
                return &( static_cast<z008fe3fade<z75c98007b4> *>( zf940a9d627 )->zb81bbfc639 );
            }

        private:
            class z1c9f89c7c0
            {
            public:
                virtual ~z1c9f89c7c0()
                {
                }

                virtual std::type_info const & type() const = 0;

                virtual z1c9f89c7c0 * clone() const = 0;
            };

            template< typename z75c98007b4 >
            class z008fe3fade : public z1c9f89c7c0
            {
            public:
                z008fe3fade( z75c98007b4 const & value )
                        : zb81bbfc639( value )
                {}

#if z97003e353b
                z008fe3fade( z75c98007b4 && value )
                        : zb81bbfc639( std::move( value ) )
                {}
#endif

                virtual std::type_info const & type() const
                {
                    return typeid( z75c98007b4 );
                }

                virtual z1c9f89c7c0 * clone() const
                {
                    return new z008fe3fade( zb81bbfc639 );
                }

                z75c98007b4 zb81bbfc639;
            };

            z1c9f89c7c0 * zf940a9d627;
        };

        inline void swap( any & x, any & y ) any_noexcept
        {
            x.swap( y );
        }

#if z97003e353b

        template< class T, class ...z7efd81ba62 >
        inline any make_any( z7efd81ba62&& ...z86fe05c556 )
        {
            return any( za62e91c7de(T), std::forward<z7efd81ba62>(z86fe05c556)...);
        }

        template< class T, class z37d26b6206, class ...z7efd81ba62 >
        inline any make_any( std::initializer_list<z37d26b6206> z46e3444520, z7efd81ba62&& ...z86fe05c556 )
        {
            return any( za62e91c7de(T), z46e3444520, std::forward<z7efd81ba62>(z86fe05c556)...);
        }

#endif 

        template<
                class z75c98007b4
#if z2fc313d89d
                , typename = typename std::enable_if< (std::is_reference<z75c98007b4>::value || std::is_copy_constructible<z75c98007b4>::value), fc::zf3831dc9b1::z7a61c02b2c >::type
#endif
        >
        zcaa661760c inline z75c98007b4 any_cast( any const & z0bfd3ce83a )
        {
            const z75c98007b4 * result = any_cast< typename z9c38ccefe1::add_const< typename z9c38ccefe1::remove_reference<z75c98007b4>::type >::type >( &z0bfd3ce83a );

#if z0d087dffd2
            assert( result );
#else
            if ( ! result )
            {
                throw bad_any_cast();
            }
#endif

            return *result;
        }

        template<
                class z75c98007b4
#if z2fc313d89d
                , typename = typename std::enable_if< (std::is_reference<z75c98007b4>::value || std::is_copy_constructible<z75c98007b4>::value), fc::zf3831dc9b1::z7a61c02b2c >::type
#endif
        >
        zcaa661760c inline z75c98007b4 any_cast( any & z0bfd3ce83a )
        {
            const z75c98007b4 * result = any_cast< typename z9c38ccefe1::remove_reference<z75c98007b4>::type >( &z0bfd3ce83a );

#if z0d087dffd2
            assert( result );
#else
            if ( ! result )
            {
                throw bad_any_cast();
            }
#endif

            return *result;
        }

#if z97003e353b

        template<
                class z75c98007b4
#if z2fc313d89d
                any_REQUIRES_T( std::is_reference<z75c98007b4>::value || std::is_copy_constructible<z75c98007b4>::value )
#endif
        >
        zcaa661760c inline z75c98007b4 any_cast( any && z0bfd3ce83a )
        {
            const z75c98007b4 * result = any_cast< typename z9c38ccefe1::remove_reference<z75c98007b4>::type >( &z0bfd3ce83a );

#if z0d087dffd2
            assert( result );
#else
            if ( ! result )
            {
                throw bad_any_cast();
            }
#endif

            return *result;
        }

#endif 

        template< class z75c98007b4 >
        zcaa661760c inline z75c98007b4 const * any_cast( any const * z0bfd3ce83a ) any_noexcept
        {
            return z0bfd3ce83a != any_nullptr && z0bfd3ce83a->type() == typeid(z75c98007b4) ? z0bfd3ce83a->z9ff1d873cd<z75c98007b4>() : any_nullptr;
        }

        template<class z75c98007b4 >
        zcaa661760c inline z75c98007b4 * any_cast( any * z0bfd3ce83a ) any_noexcept
        {
            return z0bfd3ce83a != any_nullptr && z0bfd3ce83a->type() == typeid(z75c98007b4) ? z0bfd3ce83a->z9ff1d873cd<z75c98007b4>() : any_nullptr;
        }

    
    
} 
#endif 

#endif 

