// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "RotaPrototype.h"

#include "astec_kernel/NBCAgent.h"
#include "astec_kernel/Iterator.h"
#include "astec_gaming/ASN_Messages.h"
#include "RichLabel.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: RotaPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::RotaPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver, ASN1T_MagicActionCreateObject& msg )
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

    attr_ = new ASN1T_AttrObjectROTA();
    
    attr_->agents_nbc.n = GetAgentCount();
    attr_->agents_nbc.elem  = new uint[ attr_->agents_nbc.n ];
    unsigned i = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            attr_->agents_nbc.elem[i++] = static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent* >()->GetId();

    attr_->niveau_danger = danger_->text().toUInt();
    msg_.m.attributs_specifiquesPresent = 1;
    msg_.attributs_specifiques.t        = T_AttrObjectSpecific_rota;
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
