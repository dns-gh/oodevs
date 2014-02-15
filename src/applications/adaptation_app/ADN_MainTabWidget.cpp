// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MainTabWidget.h"
#include "moc_ADN_MainTabWidget.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget constructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_MainTabWidget::ADN_MainTabWidget()
    : QTabWidget()
    , currentHistoryIndex_( -1 )
    , navigating_( false )
{
    connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( OnCurrentChanged( int ) ) );
    setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget destructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_MainTabWidget::~ADN_MainTabWidget()
{
    clear();
    history_.clear();
    elementIndexMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::AddPage
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::AddPage( E_WorkspaceElements element, QWidget& page, const QString& title )
{
    elementIndexMap_[ element ] = addTab( &page, title );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::OnBack
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::OnBack()
{
    if( currentHistoryIndex_ > 0 )
    {
        navigating_ = true;
        currentHistoryIndex_--;
        setCurrentIndex( history_[ currentHistoryIndex_ ] );
        navigating_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::OnForward
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::OnForward()
{
    if( currentHistoryIndex_ < static_cast< int >( history_.size() ) - 1 )
    {
        navigating_ = true;
        currentHistoryIndex_++;
        setCurrentIndex( history_[ currentHistoryIndex_ ] );
        navigating_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::OnCurrentChanged
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::OnCurrentChanged( int index )
{
    if( !navigating_ )
    {
        while( currentHistoryIndex_ != static_cast< int > ( history_.size() ) - 1 )
            history_.pop_back();
        history_.push_back( index );
        currentHistoryIndex_ = static_cast< int > ( history_.size() ) - 1 ;
    }
    emit BackEnabled( currentHistoryIndex_ > 0 );
    emit ForwardEnabled( currentHistoryIndex_ < static_cast< int > ( history_.size() ) - 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::OnChangeTab
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::OnChangeTab( E_WorkspaceElements target )
{
    auto it = elementIndexMap_.find( target );
    assert( it != elementIndexMap_.end() );
    setCurrentIndex( it->second );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainTabWidget::SetPageVisible
// Created: ABR 2013-10-18
// -----------------------------------------------------------------------------
void ADN_MainTabWidget::SetPageVisible( E_WorkspaceElements target, bool visible )
{
    auto it = elementIndexMap_.find( target );
    assert( it != elementIndexMap_.end() );
    widget( it->second )->setVisible( visible );
}
