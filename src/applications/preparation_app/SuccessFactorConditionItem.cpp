// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorConditionItem.h"
#include "SuccessFactorConditionsEditor.h"
#include "moc_SuccessFactorConditionItem.cpp"
#include "clients_gui/resources.h"
#include "preparation/Score_ABC.h"
#include "preparation/ScoresModel.h"
#include "preparation/SuccessFactorCondition.h"
#include "preparation/SuccessFactorConditions.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionItem::SuccessFactorConditionItem( QWidget* parent, const ScoresModel& scores )
    : Q3HBox( parent )
{
    setFixedHeight( 45 );
    setSpacing( 2 );
    setFrameStyle( Q3Frame::Panel | Q3Frame::Raised );

    {
        score_ = new gui::ValuedComboBox< const Score_ABC* >( this );
        tools::Iterator< const Score_ABC& > it( scores.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const Score_ABC& score = it.NextElement();
            score_->AddItem( score.GetName(), &score );
        }
    }
    {
        operator_ = new gui::ValuedComboBox< QString >( this );
        FillOperators();
    }
    {
        value_ = new QLineEdit( "0", this );
        value_->setValidator( new QDoubleValidator( value_ ) );
        value_->setAlignment( Qt::AlignRight );
    }
    {
        deleteButton_ = new QPushButton( MAKE_PIXMAP( trash ), "", this );
        deleteButton_->setMaximumWidth( 30 );
        connect( deleteButton_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorConditionItem::~SuccessFactorConditionItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem::FillOperators
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorConditionItem::FillOperators()
{
    operator_->AddItem( tr( "equals to" ), "eq" );
    operator_->AddItem( tr( "differs from" ), "ne" );
    operator_->AddItem( tr( "less than" ), "lt" );
    operator_->AddItem( tr( "less than or equal" ), "le" );
    operator_->AddItem( tr( "greater than" ), "gt" );
    operator_->AddItem( tr( "greater than or equal" ), "ge" );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem::StartEdit
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorConditionItem::StartEdit( const SuccessFactorCondition& condition )
{
    score_->SetCurrentItem( &condition.GetScore() );
    operator_->SetCurrentItem( condition.GetOperator().c_str() );
    value_->setText( QString::number( condition.GetValue() ) );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem::CommitTo
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorConditionItem::CommitTo( SuccessFactorConditions& conditions ) const
{
    if( score_->Count() > 0 )
        if( const Score_ABC* score = score_->GetValue() )
            conditions.AddCondition( *new SuccessFactorCondition( *score, operator_->GetValue().ascii(), value_->text().toFloat() ) );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem::EnableDeletion
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorConditionItem::EnableDeletion( bool enable )
{
    deleteButton_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditionItem::OnDelete
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorConditionItem::OnDelete()
{
    emit Deleted( *this );
}
