// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ArchitectureAttribute.h"
#include "ColorAttribute.h"
#include "InfrastructuresAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "StructureAttribute.h"
#include "UrbanObject.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Model_ABC& /*model*/, const MsgsSimToClient::MsgUrbanCreation& msg )
    : dispatcher::UrbanObject_ABC( msg.oid(), msg.name().c_str() )
    , strName_                   ( msg.name()  )
    , localisation_              ( msg.location() )
    , hasInfrastructures_        ( false )
    , hasResourceNetwork_        ( false )
{
    Initialize( msg.attributes() );
    RegisterSelf( *this );
    optionals_.localisationPresent = 0;
    optionals_.attributesPresent = 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    // NOTHING
}

#define MSG_MSG_CREATION( MSG, CLASS )     \
    if( attributes.has_##MSG##() )         \
    AddAttribute( new CLASS( attributes ) )

// -----------------------------------------------------------------------------
// Name: UrbanObject::Initialize
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
void UrbanObject::Initialize( const MsgsSimToClient::MsgUrbanAttributes& attributes )
{
    MSG_MSG_CREATION( color          , ColorAttribute );
    MSG_MSG_CREATION( architecture   , ArchitectureAttribute );
    MSG_MSG_CREATION( structure      , StructureAttribute );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::AddAttribute
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
void UrbanObject::AddAttribute( UrbanObjectAttribute_ABC* attribute )
{
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanCreation msg;
    msg().set_oid( GetId() );
    msg().set_name( strName_ );
    localisation_.Send( *msg().mutable_location() );
    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &UrbanObjectAttribute_ABC::Send, _1, boost::ref( *msg().mutable_attributes() ) ) );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UrbanUpdate msg;
    msg().set_oid( GetId() );
    if( optionals_.localisationPresent )
        localisation_.Send( *msg().mutable_location() );
    if( optionals_.attributesPresent )
        std::for_each( attributes_.begin(), attributes_.end(),
            boost::bind( &UrbanObjectAttribute_ABC::Send, _1, boost::ref( *msg().mutable_attributes() ) ) );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void UrbanObject::DoUpdate( const MsgsSimToClient::MsgUrbanUpdate& msg )
{
    if( msg.has_location() )
    {
        localisation_.Update( msg.location() );
        optionals_.localisationPresent = 1;
    }
    if( msg.has_attributes() )
    {
        if( msg.attributes().has_infrastructures() )
        {
            if( msg.attributes().infrastructures().resource_network_size() && !hasResourceNetwork_ )
            {
                hasResourceNetwork_ = true;
                AddAttribute( new ResourceNetworkAttribute( msg.attributes() ) );
            }
            if( hasInfrastructures_ )
            {
                hasInfrastructures_ = true;
                AddAttribute( new InfrastructuresAttribute( msg.attributes() ) );
            }
        }
        optionals_.attributesPresent = 1;
        std::for_each( attributes_.begin(), attributes_.end(),
            boost::bind( &UrbanObjectAttribute_ABC::Update, _1, boost::cref( msg.attributes() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Accept
// Created: SLG 2009-06-20
// -----------------------------------------------------------------------------
void UrbanObject::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Display
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
void UrbanObject::Display( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
