// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "NBCPrototype.h"
#include "clients_kernel/NBCAgent.h"
#include "tools/Iterator.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/NBCAttribute.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/LoadableSpinBox.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const tools::Resolver_ABC< NBCAgent >& resolver, int maxToxic, Object_ABC*& creation )
    : NBCPrototype_ABC( parent, resolver, maxToxic )
    , creation_( creation )
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
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        NBCAttribute* attribute = new NBCAttribute( dico );
        {
            attribute->SetState( nbcStates_->GetValue() );
            for( Q3ListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
                if( item->isSelected() )
                    attribute->AddAgent( *static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent >() );
            attribute->SetDanger( danger_->value() );
        }
        creation_->Attach( *attribute );
    }
}
