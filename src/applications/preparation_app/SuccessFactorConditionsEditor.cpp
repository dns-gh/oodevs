// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorConditionsEditor.h"
#include "moc_SuccessFactorConditionsEditor.cpp"
#include "SuccessFactorConditionItem.h"
#include "preparation/SuccessFactorCondition.h"
#include "preparation/SuccessFactorConditions.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionsEditor::SuccessFactorConditionsEditor( QWidget* parent, const ScoresModel& scores )
    : Q3VBox( parent )
    , scores_( scores )
    , scrollView_( 0 )
{
    setSpacing( 3 );
    Q3HBox* box = new Q3HBox( this );
    box->setSpacing( 5 );
    {
        operator_ = new Q3HButtonGroup( tr( "Actions must be executed when: " ), box );
        operator_->setRadioButtonExclusive( true );
        new QRadioButton( tr( "at least one condition is verified" ), operator_ ); //!< or
        new QRadioButton( tr( "all conditions are verified" ), operator_ ); //!< and
    }
    {
        QPushButton* add = new QPushButton( tr( "Add" ), box );
        add->setMaximumWidth( 60 );
        connect( add, SIGNAL( clicked() ), SLOT( CreateItem() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionsEditor::~SuccessFactorConditionsEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor::StartEdit
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorConditionsEditor::StartEdit( const SuccessFactorConditions& conditions )
{
    operator_->setButton( conditions.GetOperator() == "and" ? 1 : 0 );
    items_.clear();
    delete scrollView_;
    scrollView_ = new ScrollView( this );
    tools::Iterator< const SuccessFactorCondition& > it( conditions.CreateIterator() );
    while( it.HasMoreElements() )
        CreateItem()->StartEdit( it.NextElement() );
    if( items_.empty() )
        CreateItem();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor::CommitTo
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorConditionsEditor::CommitTo( SuccessFactorConditions& conditions ) const
{
    conditions.DeleteAll();
    conditions.SetOperator( operator_->selectedId() == 0 ? "or" : "and" );
    BOOST_FOREACH( const T_Items::value_type item, items_ )
        item->CommitTo( conditions );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor::CreateItem
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionItem* SuccessFactorConditionsEditor::CreateItem()
{
    SuccessFactorConditionItem* item = new SuccessFactorConditionItem( scrollView_->getMainWidget(), scores_ );
    scrollView_->addChild( scrollView_->getMainWidget() );
    items_.push_back( item );
    items_.front()->EnableDeletion( items_.size() > 1 );
    connect( item, SIGNAL( Deleted( SuccessFactorConditionItem& ) ), SLOT( OnDelete( SuccessFactorConditionItem& ) ) );
    item->show();
    return item;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor::OnDelete
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorConditionsEditor::OnDelete( SuccessFactorConditionItem& item )
{
    if( items_.size() > 1 )
    {
        T_Items::iterator it = std::find( items_.begin(), items_.end(), &item );
        if( it != items_.end() )
            items_.erase( it );
        item.deleteLater();
        if( items_.size() == 1 )
            items_.front()->EnableDeletion( false );
        scrollView_->addChild( scrollView_->getMainWidget() );
    }
}
