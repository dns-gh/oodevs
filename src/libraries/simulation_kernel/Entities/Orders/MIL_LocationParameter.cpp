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
#include "simulation_orders/MIL_ParameterType_Location.h"
#include "simulation_orders/MIL_ParameterType_Polygon.h"
#include "simulation_orders/MIL_ParameterType_Point.h"
#include "simulation_orders/MIL_ParameterType_LocationComposite.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter constructor
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
MIL_LocationParameter::MIL_LocationParameter( const Common::MsgLocation& asn )
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
    if( dynamic_cast< const MIL_ParameterType_Location* >( &type ) != 0 )
        return true;
    if( dynamic_cast< const MIL_ParameterType_Polygon* >( &type ) != 0 )
        return pLocalisation_->GetType() == TER_Localisation::ePolygon;
    if( dynamic_cast< const MIL_ParameterType_Point* >( &type ) != 0 )
        return pLocalisation_->GetType() == TER_Localisation::ePoint;
    if( dynamic_cast< const MIL_ParameterType_LocationComposite* >( &type ) != 0 )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToLocation
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToLocation( Common::MsgLocation& asn ) const
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
bool MIL_LocationParameter::ToPolygon( Common::MsgPolygon& asn ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePolygon )
        return false;
    NET_ASN_Tools::WriteLocation( *pLocalisation_, *asn.mutable_location() );
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

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToPoint
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToPoint( Common::MsgPoint& asn ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePoint )
        return false;
    NET_ASN_Tools::WriteLocation( *pLocalisation_, *asn.mutable_location() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToPoint
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToPoint( boost::shared_ptr< MT_Vector2D >& value ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePoint )
        return false;
    value.reset( new MT_Vector2D( pLocalisation_->GetPoints()[0] ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    ToLocation( *elem.mutable_location() );
    return true;
}
