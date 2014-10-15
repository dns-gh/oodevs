// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Federate.h"
#include <hla/Federate.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Federate constructor
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
Federate::Federate( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, const ::hla::Time_ABC& time, const ::hla::TimeInterval_ABC& lookAhead )
    : federate_( new ::hla::Federate( ambassador, name, "MASA SWORD", time, lookAhead ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Federate destructor
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
Federate::~Federate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Federate::Connect
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
bool Federate::Connect()
{
    return federate_->Connect();
}

// -----------------------------------------------------------------------------
// Name: Federate::Disconnect
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Disconnect()
{
    federate_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Federate::Create
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
bool Federate::Create( const std::string& federation, const std::string& fomFile )
{
    return federate_->Create( federation, fomFile );
}

// -----------------------------------------------------------------------------
// Name: Federate::Create
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
bool Federate::Create( const std::string& federation, const T_FomFiles& fomFiles )
{
    return federate_->Create( federation, fomFiles );
}

// -----------------------------------------------------------------------------
// Name: Federate::Destroy
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
bool Federate::Destroy( const std::string& federation )
{
    return federate_->Destroy( federation );
}

// -----------------------------------------------------------------------------
// Name: Federate::Join
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
bool Federate::Join( const std::string& federation, bool timeConstrained, bool timeRegulating )
{
    return federate_->Join( federation, timeConstrained, timeRegulating );
}

// -----------------------------------------------------------------------------
// Name: Federate::Resign
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Resign()
{
    federate_->Resign();
}

// -----------------------------------------------------------------------------
// Name: Federate::Step
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Step()
{
    federate_->Step();
}

// -----------------------------------------------------------------------------
// Name: Federate::Register
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe )
{
    federate_->Register( classID, objectClass, publish, subscribe );
}

// -----------------------------------------------------------------------------
// Name: Federate::Register
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe )
{
    federate_->Register( interactionID, interactionClass, publish, subscribe );
}

// -----------------------------------------------------------------------------
// Name: Federate::Register
// Created: SLI 2011-06-10
// -----------------------------------------------------------------------------
void Federate::Register( ::hla::FederateAmbassador_ABC& listener )
{
    federate_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: Federate::DivestRequest
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void Federate::DivestRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    federate_->NegotiatedOwnershipDivestiture( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: Federate::UnconditionalDivest
// Created: AHC 2012-04-16
// -----------------------------------------------------------------------------
void Federate::UnconditionalDivest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    federate_->UnconditionalOwnershipDivestiture( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: Federate::AcquisitionRequest
// Created: AHC 2012-03-01
// -----------------------------------------------------------------------------
void Federate::AcquisitionRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    federate_->OwnershipAcquisitionIfAvailable( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: Federate::Tick
// Created: AHC 2012-03-21
// -----------------------------------------------------------------------------
void Federate::Tick()
{
    federate_->Tick();
}

// -----------------------------------------------------------------------------
// Name: Federate::UnconditionalAcquisition
// Created: AHC 2012-04-17
// -----------------------------------------------------------------------------
void Federate::UnconditionalAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    federate_->OwnershipAcquisition( objectID, attributes, tag )
;}

// -----------------------------------------------------------------------------
// Name: Federate::GetFederateHandle
// Created: AHC 2013-03-29
// -----------------------------------------------------------------------------
const ::hla::FederateIdentifier& Federate::GetFederateHandle() const
{
    return federate_->GetFederateIdentifier();
}
