// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "RotaPrototype.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Iterator.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/ValuedListItem.h"
#include "preparation/RotaAttributes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: RotaPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::RotaPrototype( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver, kernel::Object_ABC*& creation )
    : RotaPrototype_ABC( parent, resolver )
    , creation_( creation )
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
    if( !creation_ )
        return;
    
    RotaAttributes& attr = static_cast< RotaAttributes& >( creation_->Get< RotaAttributes_ABC >() );

    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            attr.AddAgent( *static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent* >() );
    attr.SetDanger( danger_->text().toUInt() );
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Clean()
{
    // NOTHING
}
