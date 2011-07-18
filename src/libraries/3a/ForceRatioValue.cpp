// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "ForceRatioValue.h"

using namespace extractors;
using namespace sword;

namespace
{
    bool ReadMask( xml::xistream& xis, std::string type )
    {
        if( !xis.has_attribute( "types" ) )
            return true;
        FilterHelper< std::string > types( xis, "types" );
        bool result = false;
        if( types.IsAllowed( type ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ForceRatioValue constructor
// Created: FPO 2011-07-07
// -----------------------------------------------------------------------------
ForceRatioValue::ForceRatioValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ForceRatioValue constructor
// Created: FPO 2011-07-08
// -----------------------------------------------------------------------------
ForceRatioValue::ForceRatioValue( xml::xistream& xis )
    : favorable_( ReadMask ( xis, "favorable" ) )
    , neutral_( ReadMask( xis, "neutral" ) )
    , unfavorable_( ReadMask ( xis, "unfavorable" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ForceRatioValue destructor
// Created: FPO 2011-07-07
// -----------------------------------------------------------------------------
ForceRatioValue::~ForceRatioValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ForceRatioValue::Extract
// Created: FPO 2011-07-07
// -----------------------------------------------------------------------------
int ForceRatioValue::Extract( const sword::UnitAttributes& attributes )
{
    int ret = 0;

    if( favorable_ && attributes.force_ratio() == sword::ForceRatio::favorable )
        ret++;
    if( neutral_ && attributes.force_ratio() == sword::ForceRatio::neutral )
        ret++;
    if( unfavorable_ && attributes.force_ratio() == sword::ForceRatio::unfavorable )
        ret++;

    return ret;
}
