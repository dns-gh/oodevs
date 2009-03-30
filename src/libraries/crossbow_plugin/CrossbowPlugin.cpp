// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "CrossbowPlugin.h"
#include "DatabasePublisher.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "tools/MessageDispatcher_ABC.h"
#include "dispatcher/DefaultProfile.h"
#include "tools/ClientNetworker.h"
#include "game_asn/Authentication.h"

using namespace plugins;
using namespace plugins::crossbow;

namespace
{
	class DummyClientNetworker : public tools::ClientNetworker
							   , public Publisher_ABC
	{
	public:
		DummyClientNetworker()
			: defaultProfile_( new dispatcher::DefaultProfile() )
		{}
		virtual ~DummyClientNetworker() {}

		virtual void Send( const ASN1T_MsgsClientToSim& message ) {}
		virtual void Send( const ASN1T_MsgsClientToAuthentication& message )
		{
			MessageSender_ABC::Send( endpoint_, message );
		}
		virtual void Send( const ASN1T_MsgsClientToReplay& message ) {}
		virtual void Send( const ASN1T_MsgsClientToAar& message ) {}
		virtual void Send( const ASN1T_MsgsClientToMessenger& message ) {}

	protected:
		virtual void ConnectionSucceeded( const std::string& endpoint )
		{
			ClientNetworker::ConnectionSucceeded( endpoint );
			endpoint_ = endpoint;
			authentication::AuthenticationRequest message;
			message().login    = defaultProfile_->GetName().c_str();
			message().password = "";
			message.Send( *this );
			std::cout << "Dummy::ConnectionSucceeded " << endpoint << std::endl;
		}

	private:
		std::string endpoint_;
		std::auto_ptr< dispatcher::Profile_ABC > defaultProfile_;
	};
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::CrossbowPlugin( const dispatcher::Config& config, xml::xistream& xis, dispatcher::Model& model, 
                                dispatcher::SimulationPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, 
                                dispatcher::LinkResolver_ABC& links, dispatcher::CompositeRegistrable& registrables )
    : databasePublisher_( new DatabasePublisher( config, model, publisher, xis ) )
	, clientNetworker_  ( new DummyClientNetworker() )
{
	std::cout << "CrossbowPlugin::CrossbowPlugin" << std::endl;
	//clientNetworker_->Connect( "localhost:10001", false );
}
    
// -----------------------------------------------------------------------------
// Name: CrossbowPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::~CrossbowPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Update
// Created: JCR 2009-03-28
// -----------------------------------------------------------------------------
void CrossbowPlugin::Update()
{
    static bool connect = false;
    if ( !connect )
    {
        clientNetworker_->Connect( "localhost:10001", false );
        connect = true;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Receive
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::OnReceiveMessengerToClient
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPlugin::Send( const ASN1T_MsgsMessengerToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::NotifyClientAuthenticated
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::NotifyClientLeft
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}