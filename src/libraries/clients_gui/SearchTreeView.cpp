// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SearchTreeView.h"
#include "moc_SearchTreeView.cpp"
#include "RichPushButton.h"
#include "RichTreeView.h"
#include "SubObjectName.h"
#include <boost/bind.hpp>

using namespace gui;

// =============================================================================
// SearchTreeView_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC constructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
SearchTreeView_ABC::SearchTreeView_ABC( QWidget* parent )
    : QWidget( parent )
    , filtersLayout_   ( 0 )
    , currentFilterRow_( 0 )
    , signalMapper_    ( new QSignalMapper( this ) )
{
    connect( signalMapper_, SIGNAL( mapped( QWidget* ) ), this, SLOT( OnFiltersChanged( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC destructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
SearchTreeView_ABC::~SearchTreeView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::CreateGUI
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void SearchTreeView_ABC::CreateGUI( const QString& objectName )
{
    assert( treeView_ != 0 );
    SubObjectName subObject( objectName );
    // Search box
    searchLine_ = new SearchLineEdit( "searchLine", this );

    // Next Button
    RichPushButton* next = new RichPushButton( "next", QIcon( MAKE_ICON( search ) ), "", this );
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( searchLine_->height() );
    next->setMaximumHeight( searchLine_->height() );

    // Filters
    clearButton_ = new RichPushButton( "clear", tr( "Clear" ) );
    filtersContainer_ = new QWidget();
    filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
    filtersLayout_->setSpacing( 5 );
    filtersLayout_->setContentsMargins( 5, 2, 5, 2 );
    filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );

    filtersWidget_ = new ExpandableGroupBox( "filtersWidget", 0, tr( "Filters" ) );
    filtersWidget_->setVisible( false ); // $$$$ ABR 2012-06-22: Visible after the first AddFilter
    filtersWidget_->AddComponent( filtersContainer_ );

    // Connection
    connect( clearButton_, SIGNAL( clicked() ),                     this,      SLOT( OnClearFilters() ) );
    connect( searchLine_,  SIGNAL( textChanged( const QString& ) ), treeView_, SLOT( SearchAndSelect( const QString& ) ) );
    connect( searchLine_,  SIGNAL( returnPressed() ),               treeView_, SLOT( SearchAndSelectNext() ) );
    connect( next,         SIGNAL( pressed() ),                     treeView_, SLOT( SearchAndSelectNext() ) );

    // Layout
    QGridLayout* layout = new QGridLayout( this, 3, 2, 5 );
    layout->setMargin( 2 );
    layout->setSpacing( 2 );
    layout->addWidget( searchLine_, 0, 0 );
    layout->addWidget( next, 0, 1 );
    layout->addWidget( filtersWidget_, 1, 0, 1, 2 );
    layout->addWidget( treeView_, 2, 0, 1, 2 );
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::Load
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchTreeView_ABC::Load()
{
    RichTreeView* treeView = GetRichTreeView();
    if( treeView )
        treeView->CreateFilters( *this );
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::Purge
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchTreeView_ABC::Purge()
{
    RichTreeView* treeView = GetRichTreeView();
    if( treeView )
        treeView->Purge();
    // Clear ComboBoxs
    delete filtersLayout_;
    filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
    filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );
    for( std::vector< CheckComboBox* >::iterator it = combos_.begin(); it != combos_.end(); ++it )
        delete *it;
    combos_.clear();
    for( std::vector< NumericLimitsEditor_ABC* >::iterator it = numericLimits_.begin(); it != numericLimits_.end(); ++it )
        delete *it;
    numericLimits_.clear();
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::OnClearFilters
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchTreeView_ABC::OnClearFilters()
{
    QStringList list;
    for( std::vector< CheckComboBox* >::iterator it = combos_.begin(); it != combos_.end(); ++it )
        if( *it )
            ( *it )->SetItemsCheckState( Qt::Unchecked );
    for( std::vector< NumericLimitsEditor_ABC* >::iterator it = numericLimits_.begin(); it != numericLimits_.end(); ++it )
        if( *it )
            ( *it )->Clear();
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::ApplyAllFilter
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
bool SearchTreeView_ABC::ApplyAllFilter( QStandardItem& item, StandardModel& model ) const
{
    bool result = true;
    for( std::vector< CheckComboBox* >::const_iterator it = combos_.begin(); it != combos_.end(); ++it )
        if( *it )
            result = result && ( *it )->ApplyFilter( item, model );

    for( std::vector< NumericLimitsEditor_ABC* >::const_iterator it = numericLimits_.begin(); it != numericLimits_.end(); ++it )
        if( *it )
            result = result && ( *it )->ApplyFilter( item, model );

    return result;
}

// -----------------------------------------------------------------------------
// Name: SearchTreeView_ABC::OnFiltersChanged
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchTreeView_ABC::OnFiltersChanged( QWidget* )
{
    RichTreeView* treeView = GetRichTreeView();
    if( !treeView )
        return;
    treeView->ApplyFilter( boost::bind( &SearchTreeView_ABC::ApplyAllFilter, this, _1, _2 ) );
}
