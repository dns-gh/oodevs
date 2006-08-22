// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoPanels.h"
#include "moc_InfoPanels.cpp"

#include "AgentKnowledgePanel.h"
#include "ObjectKnowledgePanel.h"
#include "ObjectReportPanel.h"
#include "PopulationKnowledgePanel.h"
#include "ReportPanel.h"

#include "clients_gui/AgentStatePanel.h"
#include "clients_gui/AgentResourcesPanel.h"
#include "clients_gui/ObjectPanel.h"
#include "clients_gui/PopulationPanel.h"
#include "clients_gui/AgentMaintenancePanel.h"
#include "clients_gui/AgentMedicalPanel.h"
#include "clients_gui/AgentSupplyPanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : QVBox( pParent )
{
    setMinimumSize( 1, 1 );

    QHBox* box = new QHBox( this );
    previous_ = new QPushButton( "<", box );
    previous_->setMaximumSize( 20, 20 );
    next_ = new QPushButton( ">", box );
    next_->setMaximumSize( 20, 20 );
    combo_ = new QComboBox( box );
    combo_->setFocusPolicy( QWidget::StrongFocus );

    stack_ = new QWidgetStack( this );
    AddPanel( new AgentStatePanel         ( this, *this, controllers, factory ) );
    AddPanel( new AgentResourcesPanel     ( this, *this, controllers, factory ) );
    AddPanel( new AgentKnowledgePanel     ( this, *this, controllers, factory ) );
    AddPanel( new PopulationPanel         ( this, *this, controllers, factory ) );
    AddPanel( new PopulationKnowledgePanel( this, *this, controllers, factory ) );
    AddPanel( new ObjectPanel             ( this, *this, controllers, factory ) );
    AddPanel( new ObjectReportPanel       ( this, *this, controllers, factory ) );
    AddPanel( new ObjectKnowledgePanel    ( this, *this, controllers, factory ) );
    AddPanel( new ReportPanel             ( this, *this, controllers, factory ) );
    AddPanel( new AgentMaintenancePanel   ( this, *this, controllers, factory ) );
    AddPanel( new AgentMedicalPanel       ( this, *this, controllers, factory ) );
    AddPanel( new AgentSupplyPanel        ( this, *this, controllers, factory ) );

    dummy_ = new QWidget( stack_ );
    stack_->addWidget( dummy_ );
    stack_->raiseWidget( dummy_ );

    CheckButtons();

    connect( combo_, SIGNAL( activated( int ) ), this, SLOT( Select( int ) ) );
    connect( previous_, SIGNAL( clicked() ), this, SLOT( PreviousPage() ) );
    connect( next_, SIGNAL( clicked() ), this, SLOT( NextPage() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::AddPanel
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::AddPanel( InfoPanel_ABC* panel )
{
    stack_->addWidget( panel );
    panelStates_[panel] = false;
    panels_.push_back( panel );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::Select( int index )
{
    if( !currentPanels_.empty() && index != -1 )
    {
        stack_->raiseWidget( currentPanels_[index] );
        combo_->setCurrentItem( index );
        SaveSelection( currentPanels_[index] );
        CheckButtons();
    }
    else
        stack_->raiseWidget( dummy_ );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::Select( QWidget* widget )
{
    if( currentPanels_.empty() || !widget )
    {
        Select( -1 );
        return;
    }
    for( unsigned int i = 0; i < currentPanels_.size(); ++i )
        if( currentPanels_[i] == widget )
        {
            Select( i );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::UpdateCombo
// Created: SBO 2006-08-08  
// -----------------------------------------------------------------------------
void InfoPanels::UpdateCombo()
{
    combo_->clear();
    currentPanels_.clear();
    for( CIT_Panels it = panels_.begin(); it != panels_.end(); ++it )
        if( panelStates_[*it] )
        {
            combo_->insertItem( static_cast< InfoPanel_ABC* >( *it )->GetName() );
            currentPanels_.push_back( *it );
        }
    CheckButtons();
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Add
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanels::Add( QWidget* widget, const QString& )
{
    for( unsigned int i = 0; i < currentPanels_.size(); ++i )
        if( currentPanels_[i] == widget )
            return;

    QWidget* selected = 0;
    if( !currentPanels_.empty() )
        selected = currentPanels_[combo_->currentItem()];
    panelStates_[widget] = true;
    UpdateCombo();
    ShowPreferedWidget( selected );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::Remove
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanels::Remove( QWidget* widget )
{
    QWidget* selected = 0;
    if( !currentPanels_.empty() )
        selected = currentPanels_[combo_->currentItem()];
    panelStates_[widget] = false;
    UpdateCombo();
    ShowPreferedWidget( selected );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::sizeHint
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
QSize InfoPanels::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::ShowPreferedWidget
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
void InfoPanels::ShowPreferedWidget( QWidget* defaultSelection )
{
    IT_SelectedWidgets it = FindSelectedSet();
    if( it != widgets_.end() )
        Select( it->second );
    else
        Select( defaultSelection );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::CurrentPageChanged
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
void InfoPanels::SaveSelection( QWidget* widget )
{
    IT_SelectedWidgets it = FindSelectedSet();
    if( it == widgets_.end() )
    {
        widgets_.push_back( T_SelectedWidget() );
        for( unsigned int i = 0; i < currentPanels_.size(); ++i )
            widgets_.back().first.insert( currentPanels_[i] );
        it = widgets_.end() - 1;
    }
    it->second = widget;
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::CheckButtons
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::CheckButtons()
{
    previous_->setEnabled( combo_->currentItem() > 0 );
    next_->setEnabled( combo_->currentItem() < combo_->count() - 1 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::PreviousPage
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::PreviousPage()
{
    if( combo_->currentItem() > 0 )
        Select( combo_->currentItem() - 1 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::NextPage
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void InfoPanels::NextPage()
{
    if( combo_->currentItem() < combo_->count() - 1 )
        Select( combo_->currentItem() + 1 );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels::FindSelectedSet
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
InfoPanels::IT_SelectedWidgets InfoPanels::FindSelectedSet()
{
    T_Widgets current;
    for( unsigned int i = 0; i < currentPanels_.size(); ++i )
        current.insert( currentPanels_[i] );

    IT_SelectedWidgets it = widgets_.begin();
    while( it != widgets_.end() && it->first != current )
        ++it;
    return it;
}
