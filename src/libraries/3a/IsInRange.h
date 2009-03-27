// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IsInRange_h_
#define __IsInRange_h_

#include "Functions.h"
#include "RangeHelper.h"

// =============================================================================
/** @class  IsInRange
    @brief  IsInRange
*/
// Created: AGE 2007-09-14
// =============================================================================
template< typename K, typename T >
class IsInRange : public Function1_ABC< K, T >
{
public:
    //! @name Types
    //@{
    typedef bool Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    IsInRange( xml::xistream& xis, Function1_ABC< K, bool >& next )
        : next_ ( next )
        , filter_ ( xis, "select" )
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
    //! @name Copy/Assignment
    //@{
    IsInRange( const IsInRange& );            //!< Copy constructor
    IsInRange& operator=( const IsInRange& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "IsInRange"; }
    //@}

    //! @name Types
    //@{
    typedef std::vector< T > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, bool >& next_;
    RangeHelper< T > filter_;
    //@}
};

#endif // __IsOneOf_h_
