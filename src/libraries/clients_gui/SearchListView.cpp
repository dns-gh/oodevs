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
    combos_.clear();
    numericLimits_.clear();
}

// -----------------------------------------------------------------------------
// Name: SearchListView_ABC::OnLinkActivated
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void SearchListView_ABC::OnLinkActivated( const QString& link )
{
    if( link == "display" )
    {
        filterLabel_->setText( "<a href='hide'>" + tr( "Hide filters" ) + "</a>" );
        clearLabel_->setVisible( true );
        filtersContainer_->setVisible( true );
    }
    else if( link == "hide" )
    {
        filterLabel_->setText( "<a href='display'>" + tr( "Display filters" ) + "</a>" );
        clearLabel_->setVisible( false );
        filtersContainer_->setVisible( false );
    }
    else if( link == "clear" )
    {
        QStringList list;
        for( std::vector< CheckComboBox* >::iterator it = combos_.begin(); it != combos_.end(); ++it )
            if( *it )
                ( *it )->SetItemsCheckState( Qt::Unchecked );
        for( std::vector< NumericLimitsEditor_ABC* >::iterator it = numericLimits_.begin(); it != numericLimits_.end(); ++it )
            if( *it )
                ( *it )->Clear();
    }
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
