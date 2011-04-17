// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MT_Stl_h_
#define __MT_Stl_h_

#include <functional>

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
                // NOTHING
            }
            virtual ~unary_compose()
            {
                // NOTHING
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
                // NOTHING
            }
            virtual ~binary_compose()
            {
                // NOTHING
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
            // NOTHING
        }
        virtual ~binary_compose2()
        {
            // NOTHING
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
        // NOTHING
    }
    virtual ~class_mem_fun_void_t()
    {
        // NOTHING
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
        // NOTHING
    }
    virtual ~class_mem_fun_void_const_t()
    {
        // NOTHING
    }
    void operator()( const Param& rhs ) const
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

namespace std
{
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

#endif // __MT_Stl_h_
