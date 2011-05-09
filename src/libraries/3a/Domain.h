// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Domain_h_
#define __Domain_h_

#include "Functions.h"
#include "FilterHelper.h"

// =============================================================================
/** @class  Domain
    @brief  Domain
*/
// Created: AGE 2007-10-03
// =============================================================================
template< typename K, typename T >
class Domain : public Function1_ABC< K, T >
{
public:
    //! @name
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Domain( xml::xistream& xis, Function1_ABC< K, T >& next )
        : next_    ( next )
        , filter_  ( xis, "select" )
        , selected_( false )
    {
        // NOTHING
    }
    virtual ~Domain()
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
        selected_ = filter_.IsAllowed( key );
        if( selected_ )
            next_.SetKey( key );
    }
    virtual void Apply( const T& arg )
    {
        if( selected_ )
            next_.Apply( arg );
        selected_ = false;
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Domain"; }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& next_;
    FilterHelper< K > filter_;
    bool selected_;
    //@}
};

#endif // __Domain_h_
