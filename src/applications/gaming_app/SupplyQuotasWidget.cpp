// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyQuotasWidget.h"
#include "moc_SupplyQuotasWidget.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/tools.h"
#include "gaming/LogisticLink.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasWidget::SupplyQuotasWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& /*factory*/ )
    : Q3VBox       ( parent )
    , controllers_( controllers )
    , pLinks_( 0 )
{
    controllers_.Register( *this );

    dataModel_ = new QStandardItemModel( this );
    dataModel_->setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "LogisticSupplyChangeQuotasDialog", "Quota" )
        << tools::translate( "DotationsListView", "Quantity" );
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders );

    tableView_ = new QTableView( this );
    tableView_->setModel( dataModel_ );
    if( tableView_->verticalHeader() )
    {
        tableView_->verticalHeader()->setDefaultSectionSize( 20 );
        tableView_->verticalHeader()->hide();
    }
    if( tableView_->horizontalHeader() )
    {
        tableView_->horizontalHeader()->setFixedHeight( 20 );
        tableView_->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
        tableView_->horizontalHeader()->setDefaultAlignment( Qt::AlignLeft );
        tableView_->horizontalHeader()->setHighlightSections( false );
    }
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSortingEnabled( false );
    tableView_->setShowGrid( false );
    
    hboxSuperior_= new Q3HBox( (Q3VBox*)this );
    new QLabel( tr( "Logistic superior" ), hboxSuperior_ );
    superior_ = new ValuedComboBox< const Entity_ABC* >( hboxSuperior_ );
    superior_->setMinimumWidth( 200 );
    connect( superior_, SIGNAL( currentIndexChanged( int ) ), SLOT( UpdateQuotas() ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasWidget::~SupplyQuotasWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::UpdateQuotas
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::UpdateQuotas()
{
     if( pLinks_ )
        UpdateQuotas( *pLinks_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::UpdateSuperiors
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::UpdateSuperiors( const LogisticLinks& links )
{
    superior_->Clear();
    tools::Iterator< const LogisticLink& > it = links.CreateSuperiorLinksIterator();
    while( it.HasMoreElements() )
    {
        const LogisticLink& link = it.NextElement();
        const Entity_ABC* pCurrent = &link.GetSuperior();
        superior_->AddItem( pCurrent->GetName(), pCurrent );
    }
    if( superior_->count() > 0 )
        hboxSuperior_->show();
    else
        hboxSuperior_->hide();
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::UpdateQuotas
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::UpdateQuotas( const LogisticLinks& links )
{
    int row = 0;
    if( dataModel_->rowCount() > 0 )
        dataModel_->removeRows( 0, dataModel_->rowCount() );
    const Entity_ABC* pSuperior = superior_->Count() > 0 ? superior_->GetValue() : 0;
    if( pSuperior )
    {
        if( LogisticLink* pLink = links.FindLogisticLink( *pSuperior ) )
        {
            const std::vector< Dotation >& dotations = pLink->GetQuotas();
            for( int i=0; i<dotations.size(); ++i )
            {
                if( !dotations[ i ].type_ )
                    continue;
                QStandardItem* itemName = new QStandardItem();
                QStandardItem* itemQty = new QStandardItem();
                itemName->setEditable( false );
                itemQty->setEditable( false );
                itemName->setText( QString::fromStdString( dotations[ i ].type_->GetName() ) );
                itemQty->setText( QString::number( dotations[ i ].quantity_ ) );
                dataModel_->setItem( row, 0, itemName );
                dataModel_->setItem( row, 1, itemQty );
                ++row;
            }
        }
    }
    if( row > 0 )
        tableView_->show();
    else
        tableView_->hide();
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::NotifyUpdated( const LogisticLinks& links )
{
    if( pLinks_ == &links )
    {
        UpdateSuperiors( links );
        UpdateQuotas( links );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::NotifySelected
// Created: MMC 2012-10-03
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::NotifySelected( const Entity_ABC* pEntity )
{
    pLinks_ = 0;
    if( pEntity && EntityHelpers::IsLogisticBase( *pEntity ) )
        pLinks_ = pEntity->Retrieve< LogisticLinks >();
    if( pLinks_ )
        NotifyUpdated( *pLinks_ );
    else
    {
        hboxSuperior_->hide();
        tableView_->hide();
    }
}
