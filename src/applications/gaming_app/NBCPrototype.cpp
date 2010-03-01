// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "NBCPrototype.h"
#include "clients_kernel/NBCAgent.h"
#include "tools/Iterator.h"
#include "clients_gui/ValuedListItem.h"
#include "protocol/ClientSenders.h"

namespace MsgsClientToSim 
{
    class MsgMagicActionCreateObject;
}

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const tools::Resolver_ABC< NBCAgent >& resolver, int maxToxic, MsgsClientToSim::MsgMagicActionCreateObject& msg )
    : NBCPrototype_ABC( parent, resolver, maxToxic )
    , msg_      ( msg ) 
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: NBCPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::~NBCPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Commit()
{
    msg_.mutable_attributes()->mutable_nbc()->set_danger_level( danger_->value() );
    msg_.mutable_attributes()->mutable_nbc()->mutable_nbc_agents();
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            msg_.mutable_attributes()->mutable_nbc()->mutable_nbc_agents()->add_elem( static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent >()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Clean()
{
    if( msg_.attributes().has_nbc() )
        msg_.mutable_attributes()->clear_nbc();
}
