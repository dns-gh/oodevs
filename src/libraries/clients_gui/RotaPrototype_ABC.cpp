// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RotaPrototype_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Iterator.h"
#include "RichLabel.h"
#include "ValuedListItem.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype_ABC::RotaPrototype_ABC( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver )
    : ObjectPrototypeAttributes_ABC( parent, tools::translate( "RotaPrototype_ABC", "ROTA" ) )
    , resolver_( resolver )
{
    new QLabel( tools::translate( "RotaPrototype_ABC", "Danger level:" ), this );
    danger_ = new QSpinBox( 0, 100, 1, this );

    nbcAgentsLabel_ = new RichLabel( tools::translate( "RotaPrototype_ABC", "NBC agents:" ), this );
    nbcAgents_ = new QListView( this );
    nbcAgents_->setSelectionMode( QListView::Multi );
    nbcAgents_->setMinimumHeight( 3 * nbcAgents_->height() ); // 3 lines visible
    nbcAgents_->addColumn( tools::translate( "RotaPrototype_ABC", "Type" ) );

    FillTypes();
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype_ABC::~RotaPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void RotaPrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void RotaPrototype_ABC::FillTypes()
{
    nbcAgents_->clear();
    Iterator< const NBCAgent& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        ValuedListItem* item = new ValuedListItem( nbcAgents_ );
        item->SetNamed( element );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool RotaPrototype_ABC::CheckValidity() const
{
    if( !GetAgentCount() )
    {
        nbcAgentsLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC::GetAgentCount
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
unsigned RotaPrototype_ABC::GetAgentCount() const
{
    unsigned selected = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            ++selected;
    return selected;
}
