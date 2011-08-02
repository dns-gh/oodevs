// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::NBCPrototype_ABC */

#include "clients_gui_pch.h"
#include "NBCPrototype_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "tools/Iterator.h"
#include "RichLabel.h"
#include "ValuedListItem.h"
#include "Tools.h"
#include "LoadableSpinBox.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype_ABC::NBCPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< NBCAgent >& resolver, int maxToxic )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::NBCPrototype_ABC", "NBC parameters" ) )
    , resolver_( resolver )
    , maxToxic_( maxToxic )
{
    new QLabel( tools::translate( "gui::NBCPrototype_ABC", "Danger level:" ), this );
    danger_ = new LoadableSpinBox( 0, 10, 1, this );
    nbcAgentsLabel_ = new RichLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent(s):" ), this );
    nbcAgents_ = new Q3ListView( this );
    nbcAgents_->setMinimumHeight( 3 * nbcAgents_->height() ); // 3 lines visible
    nbcAgents_->addColumn( tools::translate( "gui::NBCPrototype_ABC", "Type" ) );


    new QLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent state:" ), this );
    nbcStates_ = new ValuedComboBox< std::string >( this );
    nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Liquid" ), std::string( "liquid" ) );
    nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Gaseous" ), std::string( "gaseous" ) );

    UpdateSelection();
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
// Name: NBCPrototype_ABC::UpdateSelection
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::UpdateSelection()
{
    if( maxToxic_ == 1 )
        nbcAgents_->setSelectionMode( Q3ListView::Single );
    else
        nbcAgents_->setSelectionMode( Q3ListView::Multi );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::FillTypes()
{
    nbcAgents_->clear();
    tools::Iterator< const NBCAgent& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        ValuedListItem* item = new ValuedListItem( nbcAgents_ );
        item->SetNamed( element );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    Q3GroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool NBCPrototype_ABC::CheckValidity() const
{
    const int count = static_cast< int >( GetAgentCount() );
    if( ( maxToxic_ != -1 && maxToxic_ != count ) || ( maxToxic_ == -1 && count == 0 ) )
    {
        nbcAgentsLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::GetAgentCount
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
unsigned NBCPrototype_ABC::GetAgentCount() const
{
    unsigned selected = 0;
    for( Q3ListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            ++selected;
    return selected;
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    danger_->SetLoader( loader );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::UpdateMaxToxic
// Created: LGY 2011-07-18
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::UpdateMaxToxic( int toxic )
{
    if( toxic > maxToxic_ )
    {
        maxToxic_ = toxic;
        UpdateSelection();
    }
}

