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
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "CheckPoints/SerializationTools.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LocationParameter )

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_LocationParameter::MIL_LocationParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter constructor
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
MIL_LocationParameter::MIL_LocationParameter( const sword::Location& asn )
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_LocationParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eLocation
        || type == MIL_ParameterType_ABC::eLocationComposite
        || ( type == MIL_ParameterType_ABC::ePolygon && pLocalisation_->GetType() == TER_Localisation::ePolygon )
        || ( type == MIL_ParameterType_ABC::ePoint && pLocalisation_->GetType() == TER_Localisation::ePoint );
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
bool MIL_LocationParameter::ToPolygon( boost::shared_ptr< TER_Localisation >& value ) const
{
    if( pLocalisation_->GetType() != TER_Localisation::ePolygon )
        return false;
    value = pLocalisation_;
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
bool MIL_LocationParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    NET_ASN_Tools::WriteLocation( *pLocalisation_, *elem.mutable_location() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::load
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_LocationParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationParameter::save
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_LocationParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pLocalisation_;
}
