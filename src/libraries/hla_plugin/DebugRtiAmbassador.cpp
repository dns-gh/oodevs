// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DebugRtiAmbassador.h"
#include "ObjectResolver_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/Time_ABC.h>
#include <hla/TimeInterval_ABC.h>
#include <hla/HLA_Lib.h>
#include <hla/AttributeFunctor_ABC.h>
#include <algorithm>

using namespace plugins::hla;
using namespace hla;

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador constructor
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
DebugRtiAmbassador::DebugRtiAmbassador( ::hla::RtiAmbassador_ABC* ambassador, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver )
    : ambassador_( ambassador )
    , logger_    ( logger )
    , resolver_  ( resolver )
    , ticking_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador destructor
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
DebugRtiAmbassador::~DebugRtiAmbassador()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Tick
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Tick()
{
    if( ! ticking_ )
        logger_.LogInfo( "-> Ticking ..." );
    ambassador_->Tick();
    ticking_ = true;
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Flush
// Created: MCO 2009-01-27
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Flush()
{
    ticking_ = false;
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Connect
// Created: SLI 2011-05-26
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::Connect( FederateAmbassador_ABC& ambassador )
{
    Flush();
    logger_.LogInfo( "-> Connect to rti" );
    return ambassador_->Connect( ambassador );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Disconnect
// Created: SLI 2011-05-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Disconnect()
{
    Flush();
    logger_.LogInfo( "-> Disconnect from rti" );
    return ambassador_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Create
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::Create( const std::string& federation, const std::string& fomFile )
{
    Flush();
    logger_.LogInfo( "-> Create federation " + federation + " with fom " + fomFile );
    return ambassador_->Create( federation, fomFile );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Create
// Created: SLI 2011-05-26
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::Create( const std::string& federation, const T_FomFiles& fomFiles )
{
    Flush();
    std::string info;
    info += "-> Create federation " + federation + " with fom files";
    std::for_each( fomFiles.begin(), fomFiles.end(), [&](const std::string& fom)
        {
            info += " " + fom;
        });
    logger_.LogInfo( info );
    return ambassador_->Create( federation, fomFiles );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Destroy
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::Destroy( const std::string& federation )
{
    Flush();
    logger_.LogInfo( "-> Destroy federation " + federation );
    return ambassador_->Destroy( federation );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Join
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::Join( const std::string& federate, const std::string& federateType, const std::string& federation )
{
    Flush();
    logger_.LogInfo( "-> Join federate " + federate + " federation " + federation );
    return ambassador_->Join( federate, federateType, federation );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Resign
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Resign()
{
    Flush();
    logger_.LogInfo( "-> Resign" );
    ambassador_->Resign();
}

namespace
{
    class AttributeFunctor : private hla::AttributeFunctor_ABC
    {
    public:
        explicit AttributeFunctor( const hla::Class_ABC& objectClass )
        {
            objectClass.Apply( *this );
        }
        virtual ~AttributeFunctor() {}
        std::string attributes;
    private:
        virtual void Visit( const hla::AttributeIdentifier& attributeID )
        {
            attributes += ( attributes.empty() ? "" : ", " ) + attributeID.ToString();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Subscribe
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Subscribe( const ClassIdentifier& classID, const Class_ABC& objectClass )
{
    Flush();
    logger_.LogInfo( "-> Subscribe class " + classID.ToString() + " { " + AttributeFunctor( objectClass ).attributes + " }" );
    ambassador_->Subscribe( classID, objectClass );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Publish
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Publish( const ClassIdentifier& classID, const Class_ABC& objectClass )
{
    Flush();
    logger_.LogInfo( "-> Publish class " + classID.ToString() + " { " + AttributeFunctor( objectClass ).attributes + " }" );
    ambassador_->Publish( classID, objectClass );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Subscribe
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Subscribe( const InteractionIdentifier& interactionID )
{
    Flush();
    logger_.LogInfo( "-> Subscribe interaction " + interactionID.ToString() );
    ambassador_->Subscribe( interactionID );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::Publish
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::Publish( const InteractionIdentifier& interactionID )
{
    Flush();
    logger_.LogInfo( "-> Publish interaction " + interactionID.ToString() );
    ambassador_->Publish( interactionID );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::ReserveObjectInstance
// Created: LGY 2010-03-05
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::ReserveObjectInstance( const std::string& name )
{
    Flush();
    logger_.LogInfo( "-> ReserveObjectInstance name " + name );
    ambassador_->ReserveObjectInstance( name );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::RegisterObjectInstance
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
ObjectIdentifier DebugRtiAmbassador::RegisterObjectInstance( const ClassIdentifier& classID, const std::string& name )
{
    Flush();
    ObjectIdentifier objectID = ambassador_->RegisterObjectInstance( classID, name );
    resolver_.Register( objectID.ToString(), name );
    logger_.LogInfo( "-> RegisterObjectInstance class '" + classID.ToString() + "' object '" + objectID.ToString() + "' name '" + name + "'" );
    return objectID;
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::DeleteObjectInstance
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::DeleteObjectInstance( const ObjectIdentifier& objectID )
{
    Flush();
    logger_.LogInfo( "-> DeleteObjectInstance object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " )" );
    resolver_.Unregister( objectID.ToString() );
    ambassador_->DeleteObjectInstance( objectID );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::RequestObjectAttributeValueUpdate
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::RequestObjectAttributeValueUpdate( const ObjectIdentifier& objectID, const Class_ABC& objectClass )
{
    Flush();
    logger_.LogInfo( "-> RequestObjectAttributeValueUpdate object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " )" );
    ambassador_->RequestObjectAttributeValueUpdate( objectID, objectClass );
}

namespace
{
    template< typename T >
    std::string ToString( const T& attributes )
    {
        std::string result;
        std::for_each( attributes.begin(), attributes.end(), [&](const T::value_type& attribute)
            {
                result += ( result.empty() ? "" : ", " ) + attribute.first.ToString();
            });
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::UpdateAttributeValues
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::UpdateAttributeValues( const ObjectIdentifier& objectID, const T_Attributes& attributes, const Time_ABC& time )
{
    Flush();
    logger_.LogInfo( "-> UpdateAttributeValues object " + resolver_.Resolve( objectID.ToString() ) + " ( " + objectID.ToString() + " ) attributes { " + ToString( attributes ) + " } time " + time.ToString() );
    ambassador_->UpdateAttributeValues( objectID, attributes, time );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::SendInteraction
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::SendInteraction( const InteractionIdentifier& interactionID, const T_Parameters& parameters, const Time_ABC& time )
{
    Flush();
    logger_.LogInfo( "-> SendInteraction object " + interactionID.ToString() + " time " + time.ToString() );
    ambassador_->SendInteraction( interactionID, parameters, time );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::EnableTimeConstrained
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::EnableTimeConstrained()
{
    Flush();
    logger_.LogInfo( "-> EnableTimeConstrained" );
    ambassador_->EnableTimeConstrained();
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::EnableTimeRegulation
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::EnableTimeRegulation( const TimeInterval_ABC& lookAhead )
{
    Flush();
    logger_.LogInfo( "-> EnableTimeRegulation lookahead " + lookAhead.ToString() );
    ambassador_->EnableTimeRegulation( lookAhead );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::NextEventRequestAvailable
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::NextEventRequestAvailable( const Time_ABC& target )
{
    if( time_ != target.ToString() )
    {
        Flush();
        logger_.LogInfo( "-> NextEventRequestAvailable time " + target.ToString() );
    }
    time_ = target.ToString();
    ambassador_->NextEventRequestAvailable( target );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::SynchronizationPointAchieved
// Created: MCO 2009-01-26
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::SynchronizationPointAchieved( const std::string& label )
{
    Flush();
    logger_.LogInfo( "-> SynchronizationPointAchieved label " + label );
    ambassador_->SynchronizationPointAchieved( label );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::RegisterSynchronizationPoint
// Created: SLI 2009-05-13
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::RegisterSynchronizationPoint( const std::string& label )
{
    Flush();
    logger_.LogInfo( "-> RegisterSynchronizationPoint label " + label );
    ambassador_->RegisterSynchronizationPoint( label );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::UnconditionalOwnershipDivestiture
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::UnconditionalOwnershipDivestiture( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes)
{
    Flush();
    logger_.LogInfo( "-> UnconditionalOwnershipDivestiture object " + objectID.ToString() );
    ambassador_->UnconditionalOwnershipDivestiture( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::NegotiatedOwnershipDivestiture
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::NegotiatedOwnershipDivestiture( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    Flush();
    logger_.LogInfo( "-> NegotiatedOwnershipDivestiture object " + objectID.ToString() );
    ambassador_->NegotiatedOwnershipDivestiture( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::ConfirmDivestiture
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::ConfirmDivestiture( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    Flush();
    logger_.LogInfo( "-> ConfirmDivestiture object " + objectID.ToString() );
    ambassador_->ConfirmDivestiture( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::OwnershipAcquisition
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::OwnershipAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    Flush();
    logger_.LogInfo( "-> OwnershipAcquisition object " + objectID.ToString() );
    ambassador_->OwnershipAcquisition( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::OwnershipAcquisitionIfAvailable
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::OwnershipAcquisitionIfAvailable( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    Flush();
    logger_.LogInfo( "-> OwnershipAcquisitionIfAvailable object " + objectID.ToString() );
    ambassador_->OwnershipAcquisitionIfAvailable( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::OwnershipDivestitureIfWanted
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::OwnershipDivestitureIfWanted( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    Flush();
    logger_.LogInfo( "-> OwnershipDivestitureIfWanted object " + objectID.ToString() );
    ambassador_->OwnershipDivestitureIfWanted( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::CancelNegotiatedOwnershipDivestiture
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::CancelNegotiatedOwnershipDivestiture( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    Flush();
    logger_.LogInfo( "-> CancelNegotiatedOwnershipDivestiture object " + objectID.ToString() );
    ambassador_->CancelNegotiatedOwnershipDivestiture( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::CancelOwnershipAcquisition
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::CancelOwnershipAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    Flush();
    logger_.LogInfo( "-> CancelOwnershipAcquisition object " + objectID.ToString() );
    ambassador_->CancelOwnershipAcquisition( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::QueryAttributeOwnership
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
void DebugRtiAmbassador::QueryAttributeOwnership( const ::hla::ObjectIdentifier& objectID, const ::hla::AttributeIdentifier& attributeID)
{
    Flush();
    logger_.LogInfo( "-> QueryAttributeOwnership object " + objectID.ToString() );
    ambassador_->QueryAttributeOwnership( objectID, attributeID );
}

// -----------------------------------------------------------------------------
// Name: DebugRtiAmbassador::IsAttributeOwnedByFederate
// Created: AHC 2012-02-24
// -----------------------------------------------------------------------------
bool DebugRtiAmbassador::IsAttributeOwnedByFederate( const ::hla::ObjectIdentifier& objectID, const ::hla::AttributeIdentifier& attributeID)
{
    Flush();
    logger_.LogInfo( "-> IsAttributeOwnedByFederate object " + objectID.ToString() );
    return ambassador_->IsAttributeOwnedByFederate( objectID, attributeID );
}
