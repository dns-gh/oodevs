// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DebugFederateAmbassador.h"
#include "ObjectResolver_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/Time_ABC.h>
#include <hla/HLA_Lib.h>
#include <boost/foreach.hpp>

using namespace plugins::hla;
using namespace hla;

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador constructor
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
DebugFederateAmbassador::DebugFederateAmbassador( dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver )
    : logger_  ( logger )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador destructor
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
DebugFederateAmbassador::~DebugFederateAmbassador()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::DiscoverObjectInstance
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::DiscoverObjectInstance( const ClassIdentifier& classID, const ObjectIdentifier& objectID, const std::string& objectName )
{
    resolver_.Register( objectID.ToString(), objectName );
    logger_.LogInfo( "<- DiscoverObjectInstance class " + classID.ToString() + " object " + objectID.ToString() + " name " + objectName );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::RemoveObjectInstance
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::RemoveObjectInstance( const ObjectIdentifier& objectID )
{
    logger_.LogInfo( "<- RemoveObjectInstance object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " )" );
    resolver_.Unregister( objectID.ToString() );
}

namespace
{
    template< typename T >
    std::string ToString( const T& attributes )
    {
        std::string result;
        BOOST_FOREACH( const T::value_type& attribute, attributes )
            result += ( result.empty() ? "" : ", " ) + attribute.first.ToString();
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::ReflectAttributeValues
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::ReflectAttributeValues( const ObjectIdentifier& objectID, const T_Attributes& attributes )
{
    logger_.LogInfo( "<- ReflectAttributeValues object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " ) attributes { " + ToString( attributes ) + " }" );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::EnableRegistrationForObjectClass
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::EnableRegistrationForObjectClass( const ClassIdentifier& classID, bool enable )
{
    logger_.LogInfo( "<- EnableRegistrationForObjectClass class " + classID.ToString() + ( enable ? " enabled " : "disabled" ) );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::ProvideAttributeValueUpdate
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::ProvideAttributeValueUpdate( const ObjectIdentifier& objectID )
{
    logger_.LogInfo( "<- ProvideAttributeValueUpdate object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " )" );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::TurnInteractions
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::TurnInteractions( const InteractionIdentifier& interactionID, bool enable )
{
    logger_.LogInfo( "<- TurnInteractions interaction " + interactionID.ToString() + ( enable ? " enabled " : "disabled" ) );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::ReceiveInteraction
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::ReceiveInteraction( const InteractionIdentifier& interactionID, const T_Parameters& /*parameters*/ )
{
    logger_.LogInfo( "<- ReceiveInteraction interaction " + interactionID.ToString() );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::TimeRegulationEnabled
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::TimeRegulationEnabled( const Time_ABC& time )
{
    logger_.LogInfo( "<- TimeRegulationEnabled time " + time.ToString() );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::TimeConstrainedEnabled
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::TimeConstrainedEnabled( const Time_ABC& time )
{
    logger_.LogInfo( "<- TimeConstrainedEnabled time " + time.ToString() );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::ReservationSucceded
// Created: LGY 2010-03-05
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::ReservationSucceded()
{
    logger_.LogInfo( "<- ReservationSucceded" );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::ReservationFailed
// Created: LGY 2010-03-05
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::ReservationFailed()
{
    logger_.LogInfo( "<- ReservationFailed" );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::TimeAdvanceGranted
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::TimeAdvanceGranted( const Time_ABC& time )
{
    if( time_ != time.ToString() )
        logger_.LogInfo( "<- TimeAdvanceGranted time " + time.ToString() );
    time_ = time.ToString();
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::AnnounceSynchronizationPoint
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::AnnounceSynchronizationPoint( const std::string& label )
{
    logger_.LogInfo( "<- AnnounceSynchronizationPoint label " + label );
}

// -----------------------------------------------------------------------------
// Name: DebugFederateAmbassador::FederationSynchronized
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugFederateAmbassador::FederationSynchronized( const std::string& label )
{
    logger_.LogInfo( "<- FederationSynchronized label " + label );
}
