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
#include "moc_NBCPrototype_ABC.cpp"
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
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::NBCPrototype_ABC", "Danger level:" ) ) );
    danger_ = new LoadableSpinBox( 0, 10, 1, this );
    layout->addWidget( danger_ );
    nbcAgentsLabel_ = new RichLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent(s):" ) );
    layout->addWidget( nbcAgentsLabel_ );
    nbcAgents_ = new Q3ListView();
    nbcAgents_->setResizeMode( Q3ListView::AllColumns );
    nbcAgents_->setMinimumHeight( 3 * nbcAgents_->height() ); // 3 lines visible
    nbcAgents_->addColumn( tools::translate( "gui::NBCPrototype_ABC", "Type" ) );
    connect( nbcAgents_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
    layout->addWidget( nbcAgents_ );

    layout->addWidget( new QLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent state:" ) ) );
    nbcStates_ = new ValuedComboBox< std::string >( this );

    layout->addWidget( nbcStates_ );

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
    RichGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool NBCPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
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

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::OnSelectionChanged
// Created: NPT 2012-07-20
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::OnSelectionChanged( Q3ListViewItem* item )
{
    nbcStates_->Clear();

    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( item );
    if( !valuedItem )
        return;
    NBCAgent* agent = valuedItem->GetValue< NBCAgent >();
    if( !agent )
        return;

    NBCAgent::T_Effects effects = agent->GetEffects();
    for( NBCAgent::CIT_Effects it = effects.begin(); it != effects.end(); ++it )
    {
        if( *it == NBCAgent::eLiquid )
            nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Liquid" ), std::string( "liquid" ) );
        else
            nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Gaseous" ), std::string( "gaseous" ) );
    }
}

