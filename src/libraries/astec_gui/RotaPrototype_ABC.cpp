// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "RotaPrototype_ABC.h"

#include "astec_kernel/NBCAgent.h"
#include "astec_kernel/Iterator.h"
#include "astec_gaming/ASN_Messages.h"
#include "RichLabel.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: RotaPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype_ABC::RotaPrototype_ABC( QWidget* parent, const Resolver< NBCAgent >& resolver )
    : ObjectPrototypeAttributes_ABC( parent, tr( "ROTA" ) )
{
    new QLabel( tr( "Danger:" ), this );
    danger_ = new QSpinBox( 0, 100, 1, this );

    nbcAgentsLabel_ = new RichLabel( tr( "Agent_ABC(s) NBC:" ), this );
    nbcAgents_ = new QListView( this );
    nbcAgents_->setSelectionMode( QListView::Multi );
    nbcAgents_->setMinimumHeight( 3 * nbcAgents_->height() ); // 3 lines visible
    nbcAgents_->addColumn( tr( "Type" ) );

    Iterator< const NBCAgent& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        ValuedListItem* item = new ValuedListItem( nbcAgents_ );
        item->Set( &element, element.GetName().c_str() );
    }
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
