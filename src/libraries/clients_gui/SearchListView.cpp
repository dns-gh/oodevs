// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SearchListView.h"
#include "moc_SearchListView.cpp"

#include "CheckComboBox.h"
#include "ListView.h"
#include <boost/bind.hpp>

using namespace gui;

// =============================================================================
// SearchListView_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC constructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
SearchListView_ABC::SearchListView_ABC( QWidget* parent )
    : QWidget( parent )
    , filtersLayout_( 0 )
    , currentFilterRow_( 0 )
    , signalMapper_( new QSignalMapper( this ) )
{
    connect( signalMapper_, SIGNAL( mapped( QWidget* ) ), this, SLOT( OnFiltersChanged( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC destructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
SearchListView_ABC::~SearchListView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::CreateGUI
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void SearchListView_ABC::CreateGUI()
{
    assert( listView_ != 0 );

    // Search box
    searchLine_ = new SearchLineEdit( this );

    // Next Button
    QPushButton* next = new QPushButton( QIcon( MAKE_ICON( search ) ), "", this );
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( searchLine_->height() );
    next->setMaximumHeight( searchLine_->height() );

    // Filters
    clearButton_ = new QPushButton( tr( "Clear" ) );
    filtersContainer_ = new QWidget();
    filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
    filtersLayout_->setSpacing( 5 );
    filtersLayout_->setContentsMargins( 5, 2, 5, 2 );
    filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );

    filtersWidget_ = new ExpandableGroupBox( 0, tr( "Filters" ) );
    filtersWidget_->setVisible( false ); // $$$$ ABR 2012-06-22: Visible after the first AddFilter
    filtersWidget_->AddComponent( filtersContainer_ );

    // Connection
    connect( clearButton_, SIGNAL( clicked() ),                     this,      SLOT( OnClearFilters() ) );
    connect( searchLine_,  SIGNAL( textChanged( const QString& ) ), listView_, SLOT( SearchAndSelect( const QString& ) ) );
    connect( searchLine_,  SIGNAL( returnPressed() ),               listView_, SLOT( SearchAndSelectNext() ) );
    connect( next,         SIGNAL( pressed() ),                     listView_, SLOT( SearchAndSelectNext() ) );

    // Layout
    QGridLayout* layout = new QGridLayout( this, 3, 2, 5 );
    layout->setMargin( 2 );
    layout->setSpacing( 2 );
    layout->addWidget( searchLine_, 0, 0 );
    layout->addWidget( next, 0, 1 );
    layout->addWidget( filtersWidget_, 1, 0, 1, 2 );
    layout->addWidget( listView_, 2, 0, 1, 2 );
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::Load
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchListView_ABC::Load()
{
    RichListView* listView = GetRichListView();
    if( listView )
        listView->CreateFilters( *this );
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::Purge
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchListView_ABC::Purge()
{
    RichListView* listView = GetRichListView();
    if( listView )
        listView->Purge();
    // Clear ComboBoxs
    delete filtersLayout_;
    filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
    filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );
    combos_.clear();
    numericLimits_.clear();
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::OnClearFilters
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchListView_ABC::OnClearFilters()
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
// Name: SearchListView_ABC::ApplyAllFilter
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
bool SearchListView_ABC::ApplyAllFilter( ValuedListItem* item ) const
{
    bool result = true;
    for( std::vector< CheckComboBox* >::const_iterator it = combos_.begin(); it != combos_.end(); ++it )
        if( *it )
            result = result && ( *it )->ApplyFilter( item );

    for( std::vector< NumericLimitsEditor_ABC* >::const_iterator it = numericLimits_.begin(); it != numericLimits_.end(); ++it )
        if( *it )
            result = result && ( *it )->ApplyFilter( item );

    return result;
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::OnFiltersChanged
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchListView_ABC::OnFiltersChanged( QWidget* )
{
    RichListView* listView = GetRichListView();
    if( !listView )
        return;
    listView->ApplyFilter( boost::bind( &SearchListView_ABC::ApplyAllFilter, this, _1 ) );
}
