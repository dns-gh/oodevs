// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Product_h_
#define __Product_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"

// =============================================================================
/** @class  Product
    @brief  Product
*/
// Created: SBO 2010-07-28
// =============================================================================
template< typename K, typename T >
class Product : public Reductor2_ABC< K, T, T >, private types::Arithmetic< T >
{
public:
    //! @name
    //@{
    typedef NumericValue Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Product( xml::xistream& /*xis*/, Function1_ABC< K, Result_Type >& next )
        : next_( next )
    {
        // NOTHING
    }
    virtual ~Product()
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
        next_.Apply( arg1 * arg2 );
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
    T result_;
    //@}
};

#endif // __Product_h_
