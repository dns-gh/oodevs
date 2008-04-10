// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "MessengerPlugin.h"
#include "IntelligenceManager.h"
#include "TacticalLineManager.h"
#include "IdManager.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Model.h"
#include "dispatcher/Config.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "tools/MessageDispatcher_ABC.h"

#include "TacticalLine_ABC.h"
#include "Intelligence.h"


#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;
using namespace messenger;

// -----------------------------------------------------------------------------
// Name: MessengerPlugin constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPlugin::MessengerPlugin( dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& dispatcher, dispatcher::LinkResolver_ABC& links, const dispatcher::Config& config)
    : config_(config)
    , clients_(clients)
    , links_(links)
    , idManager_(new IdManager)
    , converter_(new kernel::CoordinateConverter(config))
{
    tacticalLineManager_.reset(new TacticalLineManager(*idManager_,*converter_));
    intelligenceManager_.reset(new IntelligenceManager(*idManager_,*converter_));
    Load(config);
    dispatcher.RegisterMessage( *this, &MessengerPlugin::OnReceiveClientToMessenger );
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPlugin::~MessengerPlugin()
{

}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::Load
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void MessengerPlugin::Load(const tools::SessionConfig& config)
{
    if( config.HasCheckpoint())
    {
        xml::xifstream xis(GetCheckPointFileName(config.GetCheckpointDirectory()));
        xis >> xml::start( "messenger" )
                >> xml::list("automat",*this, &MessengerPlugin::ReadAutomat)
                >> xml::list("formation",*this, &MessengerPlugin::ReadFormation);
    }
    else
    {
        xml::xifstream xis( config.GetOrbatFile() );
        xis >> xml::start( "orbat" )
                >> xml::start( "sides" )
                    >> xml::list( "side", *this, &MessengerPlugin::ReadSide) ;
    }
}


// -----------------------------------------------------------------------------
// Name: MessengerPlugin::ReadSide
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void MessengerPlugin::ReadSide(xml::xistream& xis)
{
    unsigned int id ;
    xis >> xml::attribute("id",id)
        >> xml::start("tactical")
            >> xml::list("formation",*this, &MessengerPlugin::ReadFormation);
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::ReadFormation
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void MessengerPlugin::ReadFormation(xml::xistream& xis)
{
    unsigned int id ;
    xis >> xml::attribute("id",id);

    ASN1T_TacticalLinesDiffusion diffusion;
    diffusion.t = T_TacticalLinesDiffusion_formation ;
    diffusion.u.formation = id ;
    xis >> xml::list("lima",*tacticalLineManager_, &TacticalLineManager::ReadLima, diffusion);
    xis >> xml::list("limit",*tacticalLineManager_, &TacticalLineManager::ReadLimit, diffusion);

    xis >> xml::list("intelligence",*intelligenceManager_, &IntelligenceManager::ReadIntelligence, id);

    xis >> xml::list("automat",*this, &MessengerPlugin::ReadAutomat)
        >> xml::list("formation",*this, &MessengerPlugin::ReadFormation);
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::ReadAutomat
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void MessengerPlugin::ReadAutomat(xml::xistream& xis)
{
    unsigned int id ;
    xis >> xml::attribute("id",id);
    ASN1T_TacticalLinesDiffusion diffusion;
    diffusion.t = T_TacticalLinesDiffusion_automat ;
    diffusion.u.automat = id ;
    xis >> xml::list("lima",*tacticalLineManager_, &TacticalLineManager::ReadLima, diffusion);
    xis >> xml::list("limit",*tacticalLineManager_, &TacticalLineManager::ReadLimit, diffusion);
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::Receive
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::Receive( const ASN1T_MsgsSimToClient&  message)
{
    switch(message.msg.t)
    {
    case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:
        SaveCheckPoint(message.msg.u.msg_control_checkpoint_save_end->name); break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::GetCheckPointFileName
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
std::string MessengerPlugin::GetCheckPointFileName(const std::string& dir)
{
    return (bfs::path(dir,bfs::native ) / bfs::path( "messenger.xml", bfs::native )).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::SaveCheckPoint
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void MessengerPlugin::SaveCheckPoint(const std::string& name)
{
    xml::xofstream xos(GetCheckPointFileName(config_.GetCheckpointDirectory(name)));

    std::map< unsigned int , std::set< Entity_ABC* > > formations ;
    std::map< unsigned int , std::set< Entity_ABC* > > automats;

    tacticalLineManager_->CollectAutomats(automats);
    tacticalLineManager_->CollectFormations(formations);
    intelligenceManager_->CollectFormations(formations);

    xos << xml::start("messenger") ;

    std::map< unsigned int , std::set< Entity_ABC* > >::iterator it;
    for( it = formations.begin(); it != formations.end(); ++it )
    {
        xos << xml::start("formation")
            << xml::attribute("id",it->first);
            std::set< Entity_ABC*>::iterator eit ;
            for( eit = it->second.begin(); eit != it->second.end(); ++eit)
                (*eit)->Write(xos,*converter_);
        xos << xml::end();
    }
    for( it=automats.begin();it!=automats.end();++it)
    {
        xos << xml::start("automat")
            << xml::attribute("id",it->first);
            std::set< Entity_ABC*>::iterator eit ;
            for( eit = it->second.begin(); eit != it->second.end(); ++eit)
                (*eit)->Write(xos,*converter_);
        xos << xml::end();
    }
    xos << xml::end() ;

}


// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientAuthenticated
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& /*profile*/ )
{
    tacticalLineManager_->SendStateToNewClient(client);
    intelligenceManager_->SendStateToNewClient(client);
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::NotifyClientLeft
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void MessengerPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC&  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessengerPlugin::OnReceiveClientToMessenger
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
void MessengerPlugin::OnReceiveClientToMessenger(const std::string& client, const ASN1T_MsgsClientToMessenger& message)
{
    dispatcher::ClientPublisher_ABC& publisher = links_.GetPublisher( client );

    switch(message.t)
    {
    // Limit
    case T_MsgsClientToMessenger_msg_limit_creation_request:
        tacticalLineManager_->OnReceiveLimit(publisher,clients_, *message.u.msg_limit_creation_request); break;
    case T_MsgsClientToMessenger_msg_limit_destruction_request:
        tacticalLineManager_->OnReceiveLimit(publisher,clients_, message.u.msg_limit_destruction_request); break;
    case T_MsgsClientToMessenger_msg_limit_update_request:
        tacticalLineManager_->OnReceiveLimit(publisher,clients_, *message.u.msg_limit_update_request); break;
    // Lima
    case T_MsgsClientToMessenger_msg_lima_creation_request:
        tacticalLineManager_->OnReceiveLima(publisher,clients_, *message.u.msg_lima_creation_request); break;
    case T_MsgsClientToMessenger_msg_lima_destruction_request:
        tacticalLineManager_->OnReceiveLima(publisher,clients_, message.u.msg_lima_destruction_request);break;
    case T_MsgsClientToMessenger_msg_lima_update_request:
        tacticalLineManager_->OnReceiveLima(publisher,clients_, *message.u.msg_lima_update_request);break;
    // Intelligence
    case T_MsgsClientToMessenger_msg_intelligence_creation_request:
        intelligenceManager_->OnReceive(publisher,clients_,*message.u.msg_intelligence_creation_request); break;
    case T_MsgsClientToMessenger_msg_intelligence_update_request:
        intelligenceManager_->OnReceive(publisher,clients_,*message.u.msg_intelligence_update_request); break;
    case T_MsgsClientToMessenger_msg_intelligence_destruction_request:
        intelligenceManager_->OnReceive(publisher,clients_,*message.u.msg_intelligence_destruction_request); break;
    default:
        break;
    }
}
