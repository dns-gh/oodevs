// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LocationParameter.h"

#include "MIL_ParameterType_Location.h"
#include "MIL_ParameterType_Polygon.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter constructor
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
MIL_LocationParameter::MIL_LocationParameter( const ASN1T_Location& asn )
: pLocalisation_( new TER_Localisation() )
{
    NET_ASN_Tools::ReadLocation( asn, *pLocalisation_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter constructor
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
MIL_LocationParameter::MIL_LocationParameter( boost::shared_ptr< TER_Localisation > pLocalisation )
: pLocalisation_( pLocalisation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter destructor
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
MIL_LocationParameter::~MIL_LocationParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::IsOfType
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    if( dynamic_cast<const MIL_ParameterType_Location*>( &type ) != 0 )
        return true;
    if( dynamic_cast<const MIL_ParameterType_Polygon*>( &type ) != 0 )
        return ( pLocalisation_->GetType() == TER_Localisation::ePolygon );
    return false;
}
 
// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToLocation
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToLocation( ASN1T_Location& asn ) const
{
    NET_ASN_Tools::WriteLocation( *pLocalisation_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToLocation
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToLocation( boost::shared_ptr< TER_Localisation >& value ) const
{
    value = pLocalisation_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToPolygon
// Created: LDC 2009-07-27
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToPolygon( ASN1T_Polygon& asn ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePolygon )
        return false;
    NET_ASN_Tools::WriteLocation( *pLocalisation_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToPolygon
// Created: LDC 2009-07-27
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToPolygon( boost::shared_ptr< TER_Localisation >& value ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePolygon )
        return false;
    value = pLocalisation_;
    return true;
}
