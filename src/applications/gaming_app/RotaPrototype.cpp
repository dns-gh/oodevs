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
    
    attr_->agents_nbc.n = GetAgentCount();
    attr_->agents_nbc.elem  = new ASN1T_OID[ attr_->agents_nbc.n ];
    unsigned i = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            attr_->agents_nbc.elem[i++] = static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent >()->GetId();

    attr_->niveau_danger = danger_->text().toUInt();
    msg_.m.attributs_specifiquesPresent = 1;
    msg_.attributs_specifiques.t        = T_ObjectAttributesSpecific_rota;
    msg_.attributs_specifiques.u.rota   = attr_;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Clean()
{
    delete attr_; attr_ = 0;
}
