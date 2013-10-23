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
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichRadioButton.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionsEditor::SuccessFactorConditionsEditor( const QString& objectName, const ScoresModel& scores )
    : scores_( scores )
    , scrollArea_( 0 )
{
    gui::SubObjectName subObject( objectName );
    setSpacing( 3 );
    QHBoxLayout* hLayout = new QHBoxLayout;
    addLayout( hLayout );
    QGroupBox* group = new gui::RichGroupBox( "condition-groupbox", tr( "Actions must be executed when: " ) );
    hLayout->addWidget( group );
    QHBoxLayout* boxLayout = new QHBoxLayout();
    group->setLayout( boxLayout );
    boxLayout->setSpacing( 5 );
    orButton_ = new gui::RichRadioButton( "oneConditionVerified", tr( "at least one condition is verified" ) ); //!< or
    andButton_ = new gui::RichRadioButton( "allConditionsVerified", tr( "all conditions are verified" ) ); //!< and
    boxLayout->addWidget( orButton_ );
    boxLayout->addWidget( andButton_ );
    gui::RichPushButton* add = new gui::RichPushButton( "add", tr( "Add" ) );
    add->setMaximumWidth( 60 );
    connect( add, SIGNAL( clicked() ), SLOT( CreateItem() ) );
    hLayout->addWidget( add );
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
    if( conditions.GetOperator() == "and" )
        andButton_->setChecked( true );
    else 
        orButton_->setChecked( true );
    items_.clear();
    delete scrollArea_;
    QVBoxLayout* scrollLayout = new QVBoxLayout;
    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable( true );
    QWidget* widget = new QWidget;
    widget->setLayout( scrollLayout );
    scrollArea_->setWidget( widget );
    scrollLayout->setMargin( 2 );
    scrollLayout->setSpacing( 2 );
    scrollArea_->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea_->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    addWidget( scrollArea_ );
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
    conditions.SetOperator( orButton_->isChecked() ? "or" : "and" );
    BOOST_FOREACH( const T_Items::value_type item, items_ )
        item->CommitTo( conditions );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionsEditor::CreateItem
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionItem* SuccessFactorConditionsEditor::CreateItem()
{
    gui::SubObjectName subObject( "SuccessFactorConditionsEditor" );
    SuccessFactorConditionItem* item = new SuccessFactorConditionItem( "SuccessFactorConditionsEditor" + QString::number( items_.size() ) , scores_ );
    scrollArea_->widget()->layout()->addWidget( item );
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
        scrollArea_->widget()->layout()->removeWidget( &item );
    }
}
