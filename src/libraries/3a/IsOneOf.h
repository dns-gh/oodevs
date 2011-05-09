// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IsOneOf_h_
#define __IsOneOf_h_

#include "Functions.h"
#include "FilterHelper.h"

// =============================================================================
/** @class  IsOneOf
    @brief  IsOneOf
*/
// Created: AGE 2007-09-14
// =============================================================================
template< typename K, typename T >
class IsOneOf : public Function1_ABC< K, T >
{
public:
    //! @name Types
    //@{
    typedef bool Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    IsOneOf( xml::xistream& xis, Function1_ABC< K, Result_Type >& next )
        : next_  ( next )
        , filter_( xis, "select" )
    {
        // NOTHING
    }
    virtual ~IsOneOf()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    }
    virtual void Apply( const T& arg )
    {
        next_.Apply( filter_.IsAllowed( arg ) );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return "IsOneOf"; }
    //@}

    //! @name Types
    //@{
    typedef std::vector< T > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, Result_Type >& next_;
    FilterHelper< T > filter_;
    //@}
};

#endif // __IsOneOf_h_
