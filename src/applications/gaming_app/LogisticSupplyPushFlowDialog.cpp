// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyPushFlowDialog.h"
#include "moc_LogisticSupplyPushFlowDialog.cpp"
#include "gaming/AgentsModel.h"
#include "gaming/ASN_Messages.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticLinks.h"
#include "gaming/Model.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Iterator.h"
#include "clients_gui/ExclusiveComboTableItem.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Model& model )
    : QDialog( parent, "Pousser flux log" )
    , controllers_( controllers )
    , publisher_( publisher )
    , model_( model )
    , selected_( controllers )
{
    setCaption( tr( "Pousser des flux" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Cible:" ), box );
    targetCombo_ = new ValuedComboBox< const Agent_ABC* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 3, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Dotation" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quantité" ) );
    table_->horizontalHeader()->setLabel( 2, tr( "Disponible" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 300, 200 );
    table_->setColumnReadOnly( 2, true );
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
// Name: LogisticSupplyPushFlowDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::~LogisticSupplyPushFlowDialog()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type || !type->IsLogisticSupply() )
        return;
    selected_ = &agent;
    menu.InsertItem( "Commande", tr( "Pousser des flux" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Show()
{
    if( !selected_ )
        return;

    targetCombo_->Clear();
    Iterator< const Agent_ABC& > it = model_.agents_.Resolver< Agent_ABC >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const Agent_ABC& agent = it.NextElement();
        const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
        if( log && log->GetSupply() == selected_ )
            targetCombo_->AddItem( agent.GetName().c_str(), &agent );
    }
    OnSelectionChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Validate()
{
    const Agent_ABC* target = targetCombo_->GetValue();
    if( !selected_ || !target )
        return;

    ASN_MsgLogRavitaillementPousserFlux message;

    message.GetAsnMsg().oid_donneur  = selected_->GetId();
    message.GetAsnMsg().oid_receveur = target->GetId();

    unsigned int rows = 0;
    for( int i = 0; i < table_->numRows(); ++i )
        if( !table_->item( i, 0 )->text().isEmpty() )
            ++rows;

    message.GetAsnMsg().stocks.n = rows;
    if( rows > 0 )
    {
        ASN1T_DotationStock* stock = new ASN1T_DotationStock[rows];
        for( int i = 0; i < table_->numRows(); ++i )
        {
            const QString text = table_->text( i, 0 );
            if( text.isEmpty() )
                continue;
            stock[i].ressource_id        = supplies_[ text ]->type_->GetId();
            stock[i].quantite_disponible = table_->text( i, 1 ).toInt();
        }
        message.GetAsnMsg().stocks.elem = stock;
    }
    message.Send( publisher_ );
    if( message.GetAsnMsg().stocks.n > 0 )
        delete [] message.GetAsnMsg().stocks.elem;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnSelectionChanged()
{
    const Agent_ABC* agent = targetCombo_->GetValue();
    const SupplyStates* states = agent ? agent->Retrieve< SupplyStates >() : 0;
    if( !states )
        return;

    dotationTypes_.clear();
    dotationTypes_.append( "" );
    Iterator< const Dotation& > it = states->CreateIterator();
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
// Name: LogisticSupplyPushFlowDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnValueChanged( int row, int col )
{
    const Agent_ABC* agent = targetCombo_->GetValue();
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
            if( table_->numRows() < dotationTypes_.size() - 1 )
                AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
        if( ! table_->text( row, 0 ).isEmpty() )
        {
            table_->setText( row, 1, "1" );
            table_->setText( row, 2, QString::number( supplies_[ table_->text( row, 0 ) ]->quantity_ ) );
            table_->adjustColumn( 0 );
            table_->adjustColumn( 1 );
            table_->adjustColumn( 2 );
        }
        else
        {
            table_->setText( row, 1, "" );
            table_->setText( row, 2, "" );
        }
    }
    else if( col == 1 )
    {
        // $$$$ SBO 2006-07-03: check value/stock 
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddItem
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddItem()
{
    const Agent_ABC* agent = targetCombo_->GetValue();
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
