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
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/VariantPointer.h"
#include "preparation/NBCAttribute.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/LoadableSpinBox.h"
#include <tools/Iterator.h>

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
void NBCPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        gui::PropertiesDictionary& dictionary = creation_->Get< gui::PropertiesDictionary >();
        NBCAttribute* attribute = new NBCAttribute( dictionary, *creation_ );
        {
            attribute->SetState( nbcStates_->GetValue() );
            QModelIndexList indexes = nbcAgents_->selectionModel()->selectedIndexes();
            for( QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); ++it )
                attribute->AddAgent( *static_cast< const kernel::NBCAgent* >( (*it).data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ ) );
            attribute->SetDanger( danger_->value() );
        }
        creation_->Attach( *attribute );
    }
}
