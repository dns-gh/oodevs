// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Arithmetic_h_
#define __Arithmetic_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"

// =============================================================================
/** @class  Arithmetic
    @brief  Arithmetic
*/
// Created: SBO 2010-07-28
// =============================================================================
template< typename K, typename T >
class Arithmetic : public Reductor2_ABC< K, T, T >, private types::Arithmetic< T >
{
public:
    //! @name
    //@{
    typedef NumericValue Result_Type;
    typedef const std::function< T (const T&, const T& ) > Functor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Arithmetic( xml::xistream& /*xis*/, Function1_ABC< K, Result_Type >& next, Functor& f )
        : next_( next )
        , f_( f )
    {
        // NOTHING
    }
    virtual ~Arithmetic()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void OnBeginTick()
    {
        next_.BeginTick();
        result_ = T( 0 );
    }
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    }
    virtual void Apply( const T& arg1, const T& arg2 )
    {
        next_.Apply( f_( arg1, arg2 ) );
    }
    virtual void OnEndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, Result_Type >& next_;
    Functor f_;
    T result_;
    //@}
};

#endif // __Arithmetic_h_
