//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Fuseau.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: Fuseau.cpp $
//
//*****************************************************************************

#include "messenger_plugin_pch.h"

#include "TacticalLineManager.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "IdManager.h"
#include "ASN_Messages.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.h>

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: TacticalLineManager constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TacticalLineManager::TacticalLineManager(IdManager& idManager, const kernel::CoordinateConverter_ABC& converter)
    : idManager_( idManager )
    , converter_( converter )
    , limits_   ()
    , limas_    ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
TacticalLineManager::~TacticalLineManager()
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        delete it->second;
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::CreateLimit
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineManager::ReadLimit( xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion)
{
    Limit*limit = new Limit(idManager_.nextId(),xis, diffusion, converter_);
    limits_[limit->GetID()] = limit ;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineManager::ReadLima( xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion)
{
    Lima*lima = new Lima(idManager_.nextId(),xis, diffusion, converter_);
    limas_[lima->GetID()] = lima ;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLineManager::Write(xml::xostream& xos )
{
    xos << xml::start("tacticalLines");
        for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
            it->second->Write(xos,converter_);
        for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
            it->second->Write(xos,converter_);
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLimit ( creation )
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLimit( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimitCreationRequest& asn)
{
    ASN_MsgLimitCreationRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;
    Limit* limit = new Limit(idManager_.nextId(),asn);
    limits_.insert( std::make_pair( limit->GetID(), limit ) );
    limit->SendCreation( clients );
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLimit (update)
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLimit( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimitUpdateRequest& asn)
{
    ASN_MsgLimitDestructionRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;

    T_LimitMap::iterator it = limits_.find(asn.oid);
    if( it!=limits_.end())
    {
        it->second->Update(asn);
        it->second->SendUpdate(clients);
    }
    else
    {
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    }
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLimit ( destruction )
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLimit( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimitDestructionRequest& asn)
{
    ASN_MsgLimitDestructionRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;

    T_LimitMap::iterator it = limits_.find(asn);
    if( it!=limits_.end())
    {
        Limit* limit = it->second ;
        limit->SendDestruction(clients);
        delete limit;
        limits_.erase( it );
    }
    else
    {
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    }
    ack.Send(publisher);

}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLima ( creation )
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLima( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaCreationRequest& asn)
{
    ASN_MsgLimaCreationRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;
    Lima* lima = new Lima(idManager_.nextId(),asn);
    limas_[lima->GetID()] = lima ;
    lima->SendCreation(clients);
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLima (update)
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLima( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaUpdateRequest& asn)
{
    ASN_MsgLimaDestructionRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;

    T_LimaMap::iterator it = limas_.find(asn.oid);
    if( it!=limas_.end())
    {
        it->second->Update(asn);
        it->second->SendUpdate(clients);
    }
    else
    {
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    }
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::OnReceiveLima ( destruction )
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::OnReceiveLima( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgLimaDestructionRequest& asn)
{
    ASN_MsgLimaDestructionRequestAck ack ;
    ack() = EnumInfoContextErrorCode::no_error;

    T_LimaMap::iterator it = limas_.find(asn);
    if( it!=limas_.end())
    {
        Lima* lima = it->second;
        lima->SendDestruction(clients);
        delete lima;
        limas_.erase( it );
    }
    else
    {
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    }
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::SendStateToNewClient
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void TacticalLineManager::SendStateToNewClient(dispatcher::ClientPublisher_ABC& client) const
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        it->second->SendFullState(client);
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        it->second->SendFullState(client);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::CollectFormations
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLineManager::CollectFormations(T_FormationMap& formations)
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        if( it->second->GetDiffusion().t==T_TacticalLinesDiffusion_formation)
            formations[it->second->GetDiffusion().u.formation].insert(it->second);
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        if( it->second->GetDiffusion().t==T_TacticalLinesDiffusion_formation)
            formations[it->second->GetDiffusion().u.formation].insert(it->second);
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::CollectAutomats
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void TacticalLineManager::CollectAutomats(T_AutomatMap& automats)
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        if( it->second->GetDiffusion().t==T_TacticalLinesDiffusion_automat)
            automats[it->second->GetDiffusion().u.automat].insert(it->second);
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        if( it->second->GetDiffusion().t==T_TacticalLinesDiffusion_automat)
            automats[it->second->GetDiffusion().u.automat].insert(it->second);
}
