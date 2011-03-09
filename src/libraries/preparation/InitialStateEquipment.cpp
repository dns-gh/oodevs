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

// =============================================================================
// Enum conversion
// =============================================================================

namespace
{
    static InitialStateEquipment::E_EquipmentState EquipmentStateStringToEnum( const std::string& value )
    {
        if( value == "Disponible" )
            return InitialStateEquipment::eAvailable;
        else if( value == "Detruit" )
            return InitialStateEquipment::eDestroyed;
        else if( value == "ReparableAvecEvacuation" )
            return InitialStateEquipment::eBroken;
        assert( false );
        return InitialStateEquipment::eAvailable;
    }
    static const std::string EquipmentStateEnumToString( const InitialStateEquipment::E_EquipmentState& value )
    {
        if( value == InitialStateEquipment::eAvailable )
            return "Disponible";
        else if( value == InitialStateEquipment::eDestroyed )
            return "Detruit";
        else if( value == InitialStateEquipment::eBroken )
            return "ReparableAvecEvacuation";
        assert( false );
        return "";
    }
}

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
    state_ = EquipmentStateStringToEnum( state );
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
        << xml::attribute( "state", EquipmentStateEnumToString( state_ ) );
    if( state_ == eBroken )
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
