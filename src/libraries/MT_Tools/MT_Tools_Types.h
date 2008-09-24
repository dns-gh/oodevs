//*****************************************************************************
//
// $Created: FBD 02-11-15 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Tools_Types.h $
// $Author: Nld $
// $Modtime: 19/08/04 16:34 $
// $Revision: 17 $
// $Workfile: MT_Tools_Types.h $
//
//*****************************************************************************

#ifndef __MT_Tools_Types_h_
#define __MT_Tools_Types_h_

#include "MT_Assert.h"
#include "MT_Memory.h"

typedef int32       MT_COORD;
typedef double        MT_Float;
#define MT_Epsilon  1e-8
#define MT_PI 3.14159265358979323 
inline    MT_Float     MT_flabs( MT_Float rF )    { return ( rF >= 0. ? rF : -rF ); }
inline    bool         MT_IsZero( MT_Float rF )    { return MT_flabs( rF ) < MT_Epsilon; }
inline  bool         MT_IsSameSigns( MT_Float rA, MT_Float rB ) { return ( rA * rB ) >= 0.; }
#define MT_UNUSED(x) (void)x;

//-------------------------------------------------------------------------
/** @name Définition de {unary/binary}_compose ( officiellement, c'est pas dans la STL ... ) */
//-------------------------------------------------------------------------
//@{
namespace std
{
    //-----------------------------------------------------------------------------
    // Name: unary_compose
    // Created: JVT 03-05-07
    //-----------------------------------------------------------------------------
    template < class UnaryFunction1, class UnaryFunction2 >
    class unary_compose : public unary_function< typename UnaryFunction2::argument_type, typename UnaryFunction1::result_type >
    {
        public:
            unary_compose( const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
                : uf1_( uf1 )
                , uf2_( uf2 )
            {
            }

            typename UnaryFunction1::result_type operator() ( const typename UnaryFunction2::argument_type& x ) const
            {
                return uf1_( uf2_( x ) );
            }

        protected:
            UnaryFunction1  uf1_;
            UnaryFunction2  uf2_;
    };
    
    //-----------------------------------------------------------------------------
    // Name: compose2
    // Created: JVT 03-05-07
    //-----------------------------------------------------------------------------
    template < class UnaryFunction1, class UnaryFunction2 >
    inline unary_compose< UnaryFunction1, UnaryFunction2 > compose1( const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
    {
        return unary_compose< UnaryFunction1, UnaryFunction2 >( uf1, uf2 );
    }

    //-----------------------------------------------------------------------------
    // Name: binary_compose
    // Created: JVT 03-05-07
    //-----------------------------------------------------------------------------
    template < class BinaryFunction, class UnaryFunction1, class UnaryFunction2 >
    class binary_compose : public unary_function< typename UnaryFunction1::argument_type, typename BinaryFunction::result_type >
    {
        public:
            binary_compose( const BinaryFunction& bf, const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
                : bf_ ( bf )
                , uf1_( uf1 )
                , uf2_( uf2 )
            {
            }

            typename BinaryFunction::result_type operator () ( const typename UnaryFunction1::argument_type& x ) const
            {
                return bf_( uf1_( x ), uf2_( x ) );
            }

        private:
            BinaryFunction bf_;
            UnaryFunction1 uf1_;
            UnaryFunction2 uf2_;
    };

    //-----------------------------------------------------------------------------
    // Name: compose2
    // Created: JVT 03-05-07
    //-----------------------------------------------------------------------------
    template < class BinaryFunction, class UnaryFunction1, class UnaryFunction2 >
    inline binary_compose< BinaryFunction, UnaryFunction1, UnaryFunction2 > compose2( const BinaryFunction& bf, const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
    {
        return binary_compose< BinaryFunction, UnaryFunction1, UnaryFunction2 >( bf, uf1, uf2 );
    }
};
//@}


//-------------------------------------------------------------------------
/** @name Bon d'accord, je pousse pas, je le met pas dans std.... ;) */
//-------------------------------------------------------------------------
//@{
//-----------------------------------------------------------------------------
// Name: binary_compose2
// Created: JVT 03-09-15
// Last modified: JVT 03-09-22
//-----------------------------------------------------------------------------
template < class BinaryFunction, class UnaryFunction1, class UnaryFunction2 = UnaryFunction1 >
class binary_compose2 : public std::binary_function< typename UnaryFunction1::argument_type, typename UnaryFunction2::argument_type, typename BinaryFunction::result_type >
{
    public:
        binary_compose2( const BinaryFunction& bf, const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
            : bf_  ( bf )
            , uf1_ ( uf1 )
            , uf2_ ( uf2 )
        {
        }

        typename BinaryFunction::result_type operator () ( const typename UnaryFunction1::argument_type& x, const typename UnaryFunction2::argument_type& y ) const
        {
            return bf_( uf1_( x ), uf2_( y ) );
        }

    private:
        BinaryFunction bf_;
        UnaryFunction1 uf1_;
        UnaryFunction2 uf2_;
};

//-----------------------------------------------------------------------------
// Name: compose2_2
// Created: JVT 03-09-15
//-----------------------------------------------------------------------------
template < class BinaryFunction, class UnaryFunction1, class UnaryFunction2 >
inline binary_compose2< BinaryFunction, UnaryFunction1, UnaryFunction2 > compose2_2( const BinaryFunction& bf, const UnaryFunction1& uf1, const UnaryFunction2& uf2 )
{
    return binary_compose2< BinaryFunction, UnaryFunction1, UnaryFunction2 >( bf, uf1, uf2 );
}

//-----------------------------------------------------------------------------
// Name: compose2_2
// Created: JVT 03-09-22
//-----------------------------------------------------------------------------
template< class BinaryFunction, class UnaryFunction >
inline binary_compose2< BinaryFunction, UnaryFunction > compose2_2( const BinaryFunction& bf, const UnaryFunction& uf )
{
    return binary_compose2< BinaryFunction, UnaryFunction >( bf, uf, uf );
}


//-----------------------------------------------------------------------------
// Name: class_mem_fun_void_t
// Created: NLD 2004-03-17
//-----------------------------------------------------------------------------
template < class Class, class Param >
class class_mem_fun_void_t : public std::unary_function< Param&, void >
{
public:
    explicit class_mem_fun_void_t( void( Class::*fctPtr)( Param& ), Class& instance  )
        : fctPtr_   ( fctPtr )
        , pInstance_( &instance )
    {
    }

    void operator () ( Param& rhs ) const
    {
        (pInstance_->*fctPtr_)( rhs );
    }

private:
    void ( Class::*fctPtr_ )( Param& );
    Class* pInstance_;
};

//-----------------------------------------------------------------------------
// Name : const_mem_fun_ref_void
// Created: NLD 2004-03-17
//-----------------------------------------------------------------------------
template < class Class, class Param >
inline class_mem_fun_void_t< Class, Param > class_mem_fun_void( void( Class::*fctPtr)( Param& ), Class& instance )
{
    return class_mem_fun_void_t< Class, Param >( fctPtr, instance );
}

//-----------------------------------------------------------------------------
// Name: class_const_mem_fun_void_t
// Created: NLD 2004-03-17
//-----------------------------------------------------------------------------
template < class Class, class Param >
class class_mem_fun_void_const_t : public std::unary_function< const Param&, void >
{
public:
    explicit class_mem_fun_void_const_t( void( Class::*fctPtr)( const Param& ), Class& instance  )
        : fctPtr_   ( fctPtr )
        , pInstance_( &instance )
    {
    }

    void operator () ( const Param& rhs ) const
    {
        (pInstance_->*fctPtr_)( rhs );
    }

private:
    void ( Class::*fctPtr_ )( const Param& );
    Class* pInstance_;
};

//-----------------------------------------------------------------------------
// Name : const_mem_fun_ref_void
// Created: NLD 2004-03-17
//-----------------------------------------------------------------------------
template < class Class, class Param >
inline class_mem_fun_void_const_t< Class, Param > class_mem_fun_void_const( void( Class::*fctPtr)( const Param& ), Class& instance )
{
    return class_mem_fun_void_const_t< Class, Param >( fctPtr, instance );
}

//@}


//-------------------------------------------------------------------------
/** @name Fonction "identité" manquant à la STL de Visual ( bon d'accord c'est pas standard ) */
//-------------------------------------------------------------------------
//@{
namespace std
{
    //-----------------------------------------------------------------------------
    // Name: identity
    // Created: JVT 03-09-15
    //-----------------------------------------------------------------------------
    template < class type >
    struct identity : public unary_function< type, type >
    {
        const type& operator() ( const type& x ) const
        {
            return x;
        }
    };

    //-----------------------------------------------------------------------------
    // Name: project1st
    // Created: JVT 03-09-15
    //-----------------------------------------------------------------------------
    template < class Arg1, class Arg2 >
    struct project1st : public binary_function< Arg1, Arg2, Arg1 >
    {
        const Arg1& operator () ( const Arg1& x, const Arg2& y ) const
        {
            return x;
        }
    };

    //-----------------------------------------------------------------------------
    // Name: project2nd
    // Created: JVT 03-09-15
    //-----------------------------------------------------------------------------
    template < class Arg1, class Arg2 >
    struct project2nd : public binary_function< Arg1, Arg2, Arg2 >
    {
        const Arg2& operator () ( const Arg1& x, const Arg2& y ) const
        {
            return y;
        }
    };

    //-----------------------------------------------------------------------------
    // Name: select1st
    // Created: JVT 03-09-15
    //-----------------------------------------------------------------------------
    template < class Pair >
    struct select1st : public unary_function< Pair, typename Pair::first_type >
    {
        const typename Pair::first_type& operator () ( const Pair& x ) const
        {
            return x.first;
        }
    };

    //-----------------------------------------------------------------------------
    // Name: select2nd
    // Created: JVT 03-09-15
    //-----------------------------------------------------------------------------
    template < class Pair >
    struct select2nd : public unary_function< Pair, typename Pair::second_type >
    {
        const typename Pair::second_type& operator () ( const Pair& x ) const
        {
            return x.second;
        }
    };

};
//@}



//-------------------------------------------------------------------------
/** @name Comparaison de chaînes de caractère */
//-------------------------------------------------------------------------
struct sCaseInsensitiveEqual : public std::binary_function< std::string, std::string, bool >
{
    bool operator () ( const char* lhs, const char* rhs ) const
    {
        return !::_stricmp( lhs, rhs );
    }

    bool operator () ( const std::string& lhs, const std::string& rhs ) const
    {
        return operator () ( lhs.c_str(), rhs.c_str() );
    }

    bool operator () ( const char* lhs, const std::string& rhs ) const
    {
        return operator () ( lhs, rhs.c_str() );
    }

    bool operator () ( const std::string& lhs, const char* rhs ) const
    {
        return operator () ( lhs.c_str(), rhs );
    }
};

struct sCaseInsensitiveLess
{
    bool operator () ( const char* lhs, const char* rhs ) const
    {
        return ::_stricmp( lhs, rhs ) < 0;
    }

    bool operator () ( const std::string& lhs, const std::string& rhs ) const
    {
        return operator () ( lhs.c_str(), rhs.c_str() );
    }

    bool operator () ( const char* lhs, const std::string& rhs ) const
    {
        return operator () ( lhs, rhs.c_str() );
    }

    bool operator () ( const std::string& lhs, const char* rhs ) const
    {
        return operator () ( lhs.c_str(), rhs );
    }

    bool operator () ( const std::pair< std::string, std::string >& lhs, const std::pair< std::string, std::string >& rhs ) const
    {
        return ( operator()( lhs.first, rhs.first ) || !operator()( rhs.first, lhs.first) &&  operator()( lhs.second,  rhs.second ) );
    }
}; 


#endif // __MT_Tools_Types_h_