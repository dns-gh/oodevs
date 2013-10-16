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
#include "clients_kernel/VariantPointer.h"
#include "protocol/ClientSenders.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableSpinBox.h"
#include <tools/Iterator.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const tools::Resolver_ABC< NBCAgent >& resolver, int maxToxic, actions::parameters::ParameterList*& attributesList )
    : NBCPrototype_ABC( parent, resolver, maxToxic )
    , attributesList_( attributesList )
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
    actions::parameters::ParameterList& list = attributesList_->AddList( "NBC" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_nbc );
    list.AddQuantity( "DangerLevel", danger_->value() );
    actions::parameters::ParameterList& agents = list.AddList( "NBCAgents" );
    QModelIndexList indexes = nbcAgents_->selectionModel()->selectedIndexes();
    for( QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); ++it )
        agents.AddIdentifier( "Id", static_cast< const kernel::NBCAgent* >( (*it).data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ )->GetId() );
    list.AddQuantity( "State", nbcStates_->GetValue() == "liquid" ? eLiquid : eGas );
}
