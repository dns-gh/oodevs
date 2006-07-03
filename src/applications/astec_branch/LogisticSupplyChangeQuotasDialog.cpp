// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "moc_LogisticSupplyChangeQuotasDialog.cpp"
#include "Controllers.h"
#include "Model.h"
#include "Agent.h"
#include "AgentsModel.h"
#include "AutomatType.h"
#include "Dotation.h"
#include "DotationType.h"
#include "LogisticLinks.h"
#include "SupplyStates.h"
#include "Iterator.h"
#include "ASN_Messages.h"
#include "ExclusiveComboTableItem.h"

#include <qtable.h>

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, const Model& model )
    : QDialog( parent, "Affecter quotas ravitaillement" )
    , controllers_( controllers )
    , model_( model )
    , selected_( controllers )
{
    setCaption( tr( "Affecter des quotas ravitaillement" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Cible:" ), box );
    targetCombo_ = new ValuedComboBox< const Agent* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 2, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Dotation" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quantité" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 220, 200 );
    layout->addWidget( table_ );

    box = new QHBox( this );
    box->setMargin( 5 );
    QPushButton* okButton = new QPushButton( tr( "OK" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Annuler" ), box );
    okButton->setDefault( true );
    layout->addWidget( box );

    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );

    connect( targetCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSelectionChanged() ) );
    connect( table_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::~LogisticSupplyChangeQuotasDialog()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type || !type->IsLogisticSupply() )
        return;
    selected_ = &agent;
    if( menu.count() )
        menu.insertSeparator();
    menu.insertItem( tr( "Affecter des quotas ravitaillement" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Show()
{
    if( !selected_ )
        return;

    targetCombo_->Clear();
    Iterator< const Agent& > it = model_.agents_.Resolver< Agent >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const Agent& agent = it.NextElement();
        const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
        if( log && log->GetSupply() == selected_ )
            targetCombo_->AddItem( agent.GetName().c_str(), &agent );
    }
    OnSelectionChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Validate()
{
    const Agent* target = targetCombo_->GetValue();
    if( !selected_ || !target )
        return;

    ASN_MsgLogRavitaillementChangeQuotas message;

    message.GetAsnMsg().oid_donneur  = selected_->GetId();
    message.GetAsnMsg().oid_receveur = target->GetId();

    unsigned int rows = 0;
    for( int i = 0; i < table_->numRows(); ++i )
        if( !table_->item( i, 0 )->text().isEmpty() )
            ++rows;

    message.GetAsnMsg().quotas.n = rows;
    if( rows > 0 )
    {
        ASN1T_DotationQuota* quota = new ASN1T_DotationQuota[rows];
        for( int i = 0; i < table_->numRows(); ++i )
        {
            const QString text = table_->text( i, 0 );
            if( text.isEmpty() )
                continue;
            quota[i].ressource_id     = supplies_[ text ]->type_->GetId();
            quota[i].quota_disponible = table_->text( i, 1 ).toInt();
        }
        message.GetAsnMsg().quotas.elem = quota;
    }
    message.Send();
    if( message.GetAsnMsg().quotas.n > 0 )
        delete [] message.GetAsnMsg().quotas.elem;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Reject()
{
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnSelectionChanged()
{
    const Agent* agent = targetCombo_->GetValue();
    if( !agent )
        return;

    dotationTypes_.clear();
    dotationTypes_.append( "" );
    Iterator< const Dotation& > it = agent->Get< SupplyStates >().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        const QString type = dotation.type_->GetCategory().c_str();
        dotationTypes_.append( type );
        supplies_[ type ] = &dotation;
    }
    table_->setNumRows( 0 );
    AddItem();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnValueChanged( int row, int col )
{
    const Agent* agent = targetCombo_->GetValue();
    if( !selected_ || !agent )
        return;

    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( col == 0 )
    {
        if( item.currentItem() == 0 && row != table_->numRows() - 1 )
        {
            table_->removeRow( row );
            table_->setCurrentCell( table_->numRows() - 1, 1 );
            return;
        }
        if( item.currentItem() && row == table_->numRows() - 1 )
        {
            const int current = item.currentItem();
            AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
        if( ! table_->text( row, 0 ).isEmpty() )
        {
            table_->setText( row, 1, QString::number( supplies_[ table_->text( row, 0 ) ]->quantity_ ) );
            table_->adjustColumn( 0 );
            table_->adjustColumn( 1 );
        }
    }
    else if( col == 1 )
    {
        // $$$$ SBO 2006-07-03: check value/stock 
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::AddItem
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddItem()
{
    const Agent* agent = targetCombo_->GetValue();
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
