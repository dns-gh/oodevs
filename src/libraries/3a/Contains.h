// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Contains_h_
#define __Contains_h_

#include "Functions.h"
#include "Position.h"
#include "Zone.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Contains
    @brief  Contains
*/
// Created: AGE 2007-10-09
// =============================================================================
template< typename K >
class Contains : public Function2_ABC< K, ::Zone, ::Position >
{
public:
    //! @name Types
    //@{
    typedef bool Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Contains( xml::xistream&, Function1_ABC< K, bool >& next )
        : next_( next )
    {
        // NOTHING
    }
    virtual ~Contains() {}
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
    virtual void Apply( const Zone& arg1, const Position& arg2 )
    {
        next_.Apply( arg1.Contains( arg2 ) );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Contains"; }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, bool >& next_;
    //@}
};

#endif // __Contains_h_
