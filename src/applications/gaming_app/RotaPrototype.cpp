// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "RotaPrototype.h"
#include "clients_kernel/Iterator.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/ASN_Messages.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: RotaPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::RotaPrototype( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver, ASN1T_MagicActionCreateObject& msg )
    : RotaPrototype_ABC( parent, resolver )
    , msg_( msg )
    , attr_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::~RotaPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Commit()
{
    if( msg_.type != EnumObjectType::rota )
        return;

    attr_ = new ASN1T_ObjectAttributesRota();
    
    attr_->nbc_agents.n = GetAgentCount();
    attr_->nbc_agents.elem  = new ASN1T_OID[ attr_->nbc_agents.n ];
    unsigned i = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            attr_->nbc_agents.elem[i++] = static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent >()->GetId();

    attr_->danger_level = danger_->text().toUInt();
    msg_.m.specific_attributesPresent = 1;
    msg_.specific_attributes.t        = T_ObjectAttributesSpecific_rota;
    msg_.specific_attributes.u.rota   = attr_;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Clean()
{
    delete attr_; attr_ = 0;
}
