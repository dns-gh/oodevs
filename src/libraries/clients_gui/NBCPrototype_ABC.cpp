// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NBCPrototype_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Iterator.h"

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype_ABC::NBCPrototype_ABC( QWidget* parent, const Resolver< NBCAgent >& resolver )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Paramètres NBC" ) )
    , resolver_( resolver )
{
    new QLabel( tr( "Agent_ABC NBC:" ), this );
    nbcAgents_ = new ValuedComboBox< const NBCAgent* >( this );
    FillTypes();
}
    
// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype_ABC::~NBCPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::FillTypes()
{
    nbcAgents_->Clear();
    Iterator< const NBCAgent& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        nbcAgents_->AddItem( element.GetName().c_str(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool NBCPrototype_ABC::CheckValidity() const
{
    return nbcAgents_->count() && nbcAgents_->GetValue();
}
