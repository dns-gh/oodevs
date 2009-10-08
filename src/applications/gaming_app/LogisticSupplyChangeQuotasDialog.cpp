// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "moc_LogisticSupplyChangeQuotasDialog.cpp"
#include "gaming/AgentsModel.h"
#include "game_asn/SimulationSenders.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticLinks.h"
#include "gaming/Model.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DotationType.h"
#include "tools/Iterator.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/ExclusiveComboTableItem.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const Model& model, const Profile_ABC& profile )
    : QDialog( parent, tr( "Supply quotas allocation" ) )
    , controllers_( controllers )
    , publisher_( publisher )
    , model_( model )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Supply quotas allocation" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Target:" ), box );
    targetCombo_ = new ValuedComboBox< const Automat_ABC* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 2, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Dotation" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quota" ) );
//    table_->horizontalHeader()->setLabel( 2, tr( "Quota" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 220, 200 );
    layout->addWidget( table_ );

    box = new QHBox( this );
    box->setMargin( 5 );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), box );
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        const AutomatType& type = agent.GetType();
        if( type.IsLogisticSupply() )
        {
            selected_ = &agent;
            menu.InsertItem( "Command", tr( "Allocate supply quotas" ), this, SLOT( Show() ) );
        }
    }
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
    tools::Iterator< const Automat_ABC& > it = model_.agents_.Resolver< Automat_ABC >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& agent = it.NextElement();
        // $$$$ AGE 2006-08-24: 
        const LogisticLinks* log = static_cast< const LogisticLinks* >( agent.Retrieve< LogisticLinks_ABC >() );
        if( log && log->GetSupply() == selected_ )
            targetCombo_->AddItem( agent.GetName(), &agent );
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
    const Automat_ABC* target = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !target )
        return;

    simulation::LogSupplyChangeQuotas message;

    message().oid_donneur  = selected_->GetId();
    message().oid_receveur = target->GetId();

    unsigned int rows = 0;
    for( int i = 0; i < table_->numRows(); ++i )
        if( !table_->item( i, 0 )->text().isEmpty() )
            ++rows;

    message().quotas.n = rows;
    if( rows > 0 )
    {
        ASN1T_DotationQuota* quota = new ASN1T_DotationQuota[rows];
        for( int i = 0; i < table_->numRows(); ++i )
        {
            const QString text = table_->text( i, 0 );
            if( text.isEmpty() )
                continue;
            quota[i].ressource_id     = supplies_[ text ].type_->GetId();
            quota[i].quota_disponible = table_->text( i, 1 ).toInt();
        }
        message().quotas.elem = quota;
    }
    message.Send( publisher_ );
    if( message().quotas.n > 0 )
        delete [] message().quotas.elem;
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

// $$$$ AGE 2006-10-06: Factorisation avec le poussage de flux;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnSelectionChanged()
{
    supplies_.clear();
    dotationTypes_.clear();
    dotationTypes_.append( "" );
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( agent )
    {
        // $$$$ AGE 2006-10-06: use LogisticHierarchies ?
        const CommunicationHierarchies& hierarchies = agent->Get< CommunicationHierarchies >();
        tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
        
        while( children.HasMoreElements() )
            AddDotation( children.NextElement() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::AddDotation
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddDotation( const kernel::Entity_ABC& entity )
{
    const SupplyStates* states = entity.Retrieve< SupplyStates >();
    if( states )
    {
        tools::Iterator< const Dotation& > it = states->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& dotation = it.NextElement();
            const QString type = dotation.type_->GetCategory().c_str();
            Dotation& supply = supplies_[ type ];
            if( ! supply.type_ )
            {
                dotationTypes_.append( type );
                supply.type_      = dotation.type_;
            }
            supply.quantity_ += dotation.quantity_;
        }
        table_->setNumRows( 0 );
        AddItem();
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnValueChanged( int row, int col )
{
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
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
            if( table_->numRows() < int( dotationTypes_.size() ) - 1 )
                AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
//        if( ! table_->text( row, 0 ).isEmpty() )
//        {
//            const Dotation& dotation = supplies_[ table_->text( row, 0 ) ];
//            table_->setText( row, 2, QString::number( dotation.quantity_ ) );
//            table_->adjustColumn( 0 );
//            table_->adjustColumn( 1 );
//            table_->adjustColumn( 2 );
//        }
//        else
//        {
//            table_->setText( row, 1, "" );
//            table_->setText( row, 2, "" );
//        }
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
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
