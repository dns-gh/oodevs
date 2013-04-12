// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CriticalIntelligence.h"
#include "PropertiesDictionary.h"
#include "DictionaryUpdated.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence constructor
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
CriticalIntelligence::CriticalIntelligence( Entity_ABC& entity, Controller& controller,
                                            PropertiesDictionary& dictionary )
    : controller_( controller )
    , entity_    ( entity )
    , property_  ( tools::translate( "CriticalIntelligence", "Info/Critical intelligence" ) )
{
    dictionary.Register( entity, property_, value_ );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence constructor
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
CriticalIntelligence::CriticalIntelligence( xml::xistream& xis, Controller& controller, Entity_ABC& entity,
                                            PropertiesDictionary& dictionary )
    : controller_( controller )
    , entity_    ( entity )
    , property_  ( tools::translate( "CriticalIntelligence", "Info/Critical intelligence" ) )
{
    std::string value = "";
    xis >> xml::optional
        >> xml::start( "critical-intelligence" )
            >> xml::attribute( "content", value )
        >> xml::end;
    value_ = value.c_str();
    dictionary.Register( entity, property_, value_ );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence destructor
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
CriticalIntelligence::~CriticalIntelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence::SerializeAttributes
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void CriticalIntelligence::SerializeAttributes( xml::xostream& xos ) const
{
    if( value_() != "" )
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", value_().toStdString() )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence::DoUpdate
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void CriticalIntelligence::DoUpdate( const sword::CrowdUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence::DoUpdate
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void CriticalIntelligence::DoUpdate( const sword::UnitAttributes& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence::UpdateData
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
template< typename T >
void CriticalIntelligence::UpdateData( const T& message )
{
    if( message.has_critical_intelligence() )
    {
        std::string value = message.critical_intelligence().c_str();
        if( value.c_str() != value_() )
        {
            value_ = value.c_str();
            controller_.Update( DictionaryUpdated( entity_, property_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CriticalIntelligence::GetType
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
kernel::CriticalIntelligenceType& CriticalIntelligence::GetType()
{
    return value_;
}
