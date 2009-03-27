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
#include "clients_kernel/Iterator.h"
#include "clients_gui/ValuedListItem.h"
#include "game_asn/SimulationSenders.h"


using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver, int maxToxic, ASN1T_MagicActionCreateObject& msg )
    : NBCPrototype_ABC( parent, resolver, maxToxic )
    , msg_      ( msg ) 
{
    msg_.attributes.nbc.nbc_agents.n = 0;
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
    msg_.attributes.m.nbcPresent = 1;
    msg_.attributes.nbc.nbc_agents.n = GetAgentCount();
    msg_.attributes.nbc.nbc_agents.elem = new ASN1T_OID[ msg_.attributes.nbc.nbc_agents.n ];
    
    unsigned i = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            msg_.attributes.nbc.nbc_agents.elem[ i++ ] = static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent >()->GetId();
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Clean()
{
    msg_.attributes.m.nbcPresent = 0;
    if ( msg_.attributes.nbc.nbc_agents.n > 0 )
        delete [] msg_.attributes.nbc.nbc_agents.elem;
    msg_.attributes.nbc.nbc_agents.n = 0;
}
