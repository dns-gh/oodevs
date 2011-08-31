// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InitialStateEquipment.h"
#include <xeumeuleu/xml.hpp>
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::InitialStateEquipment
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateEquipment::InitialStateEquipment( const QString& name, E_EquipmentState state, const QStringList& breakdowns, unsigned int currentBreakdown /*= 0*/ )
    : name_            ( name )
    , state_           ( state )
    , breakdowns_      ( breakdowns )
    , currentBreakdown_( currentBreakdown )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::InitialStateEquipment
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateEquipment::InitialStateEquipment( xml::xistream& xis )
    : currentBreakdown_( 0 )
{
    std::string name;
    std::string state;
    xis >> xml::attribute( "type", name )
        >> xml::attribute( "state", state );
    name_ = name.c_str();
    state_ = ENT_Tr::ConvertToEquipmentState( state );
}

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::~InitialStateEquipment
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateEquipment::~InitialStateEquipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::Serialize
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
void InitialStateEquipment::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "equipment" )
        << xml::attribute( "type", name_ )
        << xml::attribute( "state", ENT_Tr::ConvertFromEquipmentState( state_, ENT_Tr_ABC::eToSim ) );
    if( state_ == eEquipmentState_RepairableWithEvacuation && !breakdowns_.isEmpty() && static_cast< int >( currentBreakdown_ ) < breakdowns_.size() )
        xos << xml::attribute( "breakdown", breakdowns_[ currentBreakdown_ ] );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::operator==
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateEquipment::operator==( const InitialStateEquipment& object ) const
{
    return name_ == object.name_ && state_ == object.state_ && currentBreakdown_ == object.currentBreakdown_ && breakdowns_ == object.breakdowns_;
}

// -----------------------------------------------------------------------------
// Name: InitialStateEquipment::operator!=
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateEquipment::operator!=( const InitialStateEquipment& object ) const
{
    return !( *this == object );
}
