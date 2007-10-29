// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Resources.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: Resources constructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
Resources::Resources()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Resources constructor
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
Resources::Resources( xml::xistream& xis )
    : filter_( xis, "dotations" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Resources::Extract
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
int Resources::Extract( const ASN1T_MsgUnitAttributes& attributes )
{
    unsigned size = attributes.dotation_eff_ressource.n;
    while( size > 0 )
    {
        --size;
        const int dotation = attributes.dotation_eff_ressource.elem[ size ].ressource_id;
        const int quantity = attributes.dotation_eff_ressource.elem[ size ].quantite_disponible;
        if( filter_.IsAllowed( dotation ) )
            resources_[ dotation ] = quantity;
    }
    int result = 0;
    for( std::map< int, int >::const_iterator it = resources_.begin(); it != resources_.end(); ++it )
        result += it->second;
    return result;
}
