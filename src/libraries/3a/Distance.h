// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Distance_h_
#define __Distance_h_

#include "Functions.h"
#include "Position.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Distance
    @brief  Distance
*/
// Created: AGE 2007-09-12
// =============================================================================
template< typename K >
class Distance : public Function2_ABC< K, Position, Position >
{
public:
    //! @name Types
    //@{
    typedef NumericValue Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Distance( xml::xistream&, Function1_ABC< K, NumericValue >& next )
        : next_( next )
    {
        // NOTHING
    }
    virtual ~Distance()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    };
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    };
    virtual void Apply( const Position& arg1, const Position& arg2 )
    {
        next_.Apply( arg1.Distance( arg2 ) );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    };
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Distance"; }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, NumericValue >& next_;
    //@}
};

#endif // __Distance_h_
