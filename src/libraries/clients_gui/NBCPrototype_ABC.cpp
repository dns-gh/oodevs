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
#include "clients_kernel/VariantPointer.h"
#include "RichLabel.h"
#include "Tools.h"
#include "LoadableSpinBox.h"
#include "SubObjectName.h"
#include <tools/Iterator.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype_ABC::NBCPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< NBCAgent >& resolver, int maxToxic )
    : ObjectAttributePrototype_ABC( parent, "NBCPrototypeABC", tools::translate( "gui::NBCPrototype_ABC", "NBC parameters" ) )
    , resolver_ ( resolver )
    , maxToxic_ ( maxToxic )
    , model_    ( new QStandardItemModel( this ) )
    , nbcAgents_( new QTreeView() )
{
    gui::SubObjectName subObject( "NBCPrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::NBCPrototype_ABC", "Danger level:" ) ) );
    danger_ = new LoadableSpinBox( "danger", 0, 10, 1, this );
    layout->addWidget( danger_ );
    nbcAgentsLabel_ = new RichLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent(s):" ) );
    layout->addWidget( nbcAgentsLabel_ );

    nbcAgents_->setModel( model_ );
    nbcAgents_->header()->hide();

    layout->addWidget( nbcAgents_ );

    layout->addWidget( new QLabel( tools::translate( "gui::NBCPrototype_ABC", "NBC agent state:" ) ) );
    nbcStates_ = new ValuedComboBox< std::string >( "nbcStates", this );
    nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Liquid" ), std::string( "liquid" ) );
    nbcStates_->AddItem( tools::translate( "gui::NBCPrototype_ABC", "Gaseous" ), std::string( "gaseous" ) );
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
    nbcAgents_->setSelectionMode( maxToxic_ == 1 ? QAbstractItemView::SingleSelection :
                                                   QAbstractItemView::MultiSelection );
}

namespace
{
    QStandardItem* CreateItem( const kernel::NBCAgent& agent )
    {
        QStandardItem* item = new QStandardItem( agent.GetName().c_str() );
        item->setEditable( false );
        QVariant* variant = new QVariant();
        variant->setValue( kernel::VariantPointer( &agent ) );
        item->setData( *variant, Qt::UserRole );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void NBCPrototype_ABC::FillTypes()
{
    model_->clear();
    tools::Iterator< const NBCAgent& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
        model_->appendRow( CreateItem( it.NextElement() ) );
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
    return nbcAgents_->selectionModel()->selectedIndexes().size();
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
