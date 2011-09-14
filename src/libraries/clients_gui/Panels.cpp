// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Panels.h"
#include "moc_Panels.cpp"
#include "clients_gui/InfoPanel_ABC.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Panels constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Panels::Panels( QWidget* pParent )
    : Q3VBox( pParent )
{
    setMinimumSize( 1, 1 );

    Q3HBox* box = new Q3HBox( this );
    box->setMaximumHeight( 20 );
    previous_ = new QPushButton( box );
    previous_->setPixmap( MAKE_PIXMAP( left_arrow ) );
    previous_->setMaximumSize( 20, 20 );
    previous_->setFlat( true );
    next_ = new QPushButton( box );
    next_->setPixmap( MAKE_PIXMAP( right_arrow ) );
    next_->setMaximumSize( 20, 20 );
    next_->setFlat( true );
    combo_ = new QComboBox( box );
    combo_->setFocusPolicy( Qt::StrongFocus );

    stack_ = new QStackedWidget( this );

    dummy_ = new QWidget();
    stack_->addWidget( dummy_ );
    stack_->setCurrentWidget( dummy_ );

    CheckButtons();

    connect( combo_, SIGNAL( activated( int ) ), this, SLOT( Select( int ) ) );
    connect( previous_, SIGNAL( clicked() ), this, SLOT( PreviousPage() ) );
    connect( next_, SIGNAL( clicked() ), this, SLOT( NextPage() ) );
}

// -----------------------------------------------------------------------------
// Name: Panels destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Panels::~Panels()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Panels::AddPanel
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::AddPanel( InfoPanel_ABC* panel )
{
    stack_->addWidget( panel );
    panelStates_[panel] = false;
    panels_.push_back( panel );
    panel->hide();
}

// -----------------------------------------------------------------------------
// Name: Panels::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::Select( int index )
{
    if( index != -1 && index < int( currentPanels_.size() ) )
    {
        stack_->setCurrentWidget( currentPanels_.at( index ) );
        combo_->setCurrentItem( index );
        SaveSelection( stack_->currentWidget() );
        stack_->currentWidget()->show();
        CheckButtons();
    }
    else
        stack_->setCurrentWidget( dummy_ );
}

// -----------------------------------------------------------------------------
// Name: Panels::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::Select( QWidget* widget )
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
// Name: Panels::UpdateCombo
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::UpdateCombo()
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
// Name: Panels::Add
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void Panels::Add( QWidget* widget )
{
    for( unsigned int i = 0; i < currentPanels_.size(); ++i )
        if( currentPanels_[i] == widget )
            return;

    QWidget* selected = 0;
    if( !currentPanels_.empty() )
        selected = currentPanels_[combo_->currentItem()];
    else
        selected = widget;
    panelStates_[widget] = true;
    UpdateCombo();
    ShowPreferedWidget( selected );
}

// -----------------------------------------------------------------------------
// Name: Panels::Remove
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void Panels::Remove( QWidget* widget )
{
    QWidget* selected = 0;
    if( !currentPanels_.empty() )
        selected = currentPanels_[combo_->currentItem()];
    panelStates_[widget] = false;
    UpdateCombo();
    ShowPreferedWidget( selected );
}

// -----------------------------------------------------------------------------
// Name: Panels::sizeHint
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
QSize Panels::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: Panels::ShowPreferedWidget
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
void Panels::ShowPreferedWidget( QWidget* defaultSelection )
{
    IT_SelectedWidgets it = FindSelectedSet();
    if( it != widgets_.end() )
        Select( it->second );
    else
        Select( defaultSelection );
}

// -----------------------------------------------------------------------------
// Name: Panels::CurrentPageChanged
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
void Panels::SaveSelection( QWidget* widget )
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
// Name: Panels::CheckButtons
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::CheckButtons()
{
    previous_->setEnabled( combo_->currentItem() > 0 );
    next_->setEnabled( combo_->currentItem() < combo_->count() - 1 );
}

// -----------------------------------------------------------------------------
// Name: Panels::PreviousPage
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::PreviousPage()
{
    if( combo_->currentItem() > 0 )
        Select( combo_->currentItem() - 1 );
}

// -----------------------------------------------------------------------------
// Name: Panels::NextPage
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Panels::NextPage()
{
    if( combo_->currentItem() < combo_->count() - 1 )
        Select( combo_->currentItem() + 1 );
}

// -----------------------------------------------------------------------------
// Name: Panels::FindSelectedSet
// Created: AGE 2006-05-22
// -----------------------------------------------------------------------------
Panels::IT_SelectedWidgets Panels::FindSelectedSet()
{
    T_Widgets current;
    for( unsigned int i = 0; i < currentPanels_.size(); ++i )
        current.insert( currentPanels_[i] );

    IT_SelectedWidgets it = widgets_.begin();
    while( it != widgets_.end() && it->first != current )
        ++it;
    return it;
}
