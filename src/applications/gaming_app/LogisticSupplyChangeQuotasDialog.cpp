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
#include "astec_gaming/AgentsModel.h"
#include "astec_gaming/ASN_Messages.h"
#include "astec_gaming/Dotation.h"
#include "astec_gaming/LogisticLinks.h"
#include "astec_gaming/Model.h"
#include "astec_gaming/SupplyStates.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/AutomatType.h"
#include "astec_kernel/DotationType.h"
#include "astec_kernel/Iterator.h"
#include "astec_gui/ExclusiveComboTableItem.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Model& model )
    : QDialog( parent, "Affecter quotas ravitaillement" )
    , controllers_( controllers )
    , publisher_( publisher )
    , model_( model )
    , selected_( controllers )
{
    setCaption( tr( "Affecter des quotas ravitaillement" ) );
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
    table_->horizontalHeader()->setLabel( 2, tr( "Quota" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 220, 200 );
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
void LogisticSupplyChangeQuotasDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    const AutomatType* type = agent.GetAutomatType();
    if( !type || !type->IsLogisticSupply() )
        return;
    selected_ = &agent;
    menu.InsertItem( "Commande", tr( "Affecter des quotas ravitaillement" ), this, SLOT( Show() ) );
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
// Name: LogisticSupplyChangeQuotasDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Validate()
{
    const Agent_ABC* target = targetCombo_->GetValue();
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
    message.Send( publisher_ );
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
        supplies_[ type ] = 0;
    }

    for( std::vector< Dotation >::const_iterator it = states->quotas_.begin(); it != states->quotas_.end(); ++it )
        supplies_[ it->type_->GetCategory().c_str() ] = &*it;

    table_->setNumRows( 0 );
    AddItem();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnValueChanged( int row, int col )
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
            const Dotation* dotation = supplies_[ table_->text( row, 0 ) ];
            table_->setText( row, 2, QString::number( dotation ? dotation->quantity_ : 0 ) );
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
// Name: LogisticSupplyChangeQuotasDialog::AddItem
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddItem()
{
    const Agent_ABC* agent = targetCombo_->GetValue();
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
