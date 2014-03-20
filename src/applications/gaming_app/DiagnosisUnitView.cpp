// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DiagnosisUnitView.h"
#include "moc_DiagnosisUnitView.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_gui/InternalLinks.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/Roles.h"

namespace
{
    enum E_Column
    {
        eName,
        eTime,
        eNbrColumn,
    };
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView constructor
// Created: SLI 2014-03-17
// -----------------------------------------------------------------------------
DiagnosisUnitView::DiagnosisUnitView( QWidget* parent, gui::DisplayExtractor& extractor )
    : gui::RichWidget< QTreeView >( "manual_selection_repair_team_DiagnosisUnitView", parent )
    , extractor_( extractor )
{
    const QStringList headers = QStringList()
        << tr( "Unit" )
        << tr( "Time to travel" );
    model_ = new QStandardItemModel( this );
    model_->setHorizontalHeaderLabels( headers );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setModel( model_ );
    setSortingEnabled( false );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    gui::LinkItemDelegate* linkItemDelegate = new gui::LinkItemDelegate( this );
    connect( linkItemDelegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    setItemDelegateForColumn( eName, linkItemDelegate );
    setEditTriggers( AllEditTriggers );
    header()->setHighlightSections( false );
    header()->setStretchLastSection( false );
    header()->setResizeMode( eName, QHeaderView::Stretch );
    header()->setResizeMode( eTime, QHeaderView::ResizeToContents );
    connect( selectionModel(), SIGNAL( currentRowChanged( const QModelIndex&, const QModelIndex& ) ),
             this,             SLOT( OnSelectionChanged( const QModelIndex&, const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView destructor
// Created: SLI 2014-03-17
// -----------------------------------------------------------------------------
DiagnosisUnitView::~DiagnosisUnitView()
{
    // NOTHING
}

namespace
{
    QString ApproximateTimeTravel( const kernel::Entity_ABC& from, const kernel::Agent_ABC& to, const kernel::EquipmentType& carrier )
    {
        const geometry::Point2f fromPosition = from.Get< kernel::Positions >().GetPosition();
        const geometry::Point2f toPosition = to.Get< kernel::Positions >().GetPosition();
        const unsigned int duration = static_cast< unsigned int >( 1.439 * fromPosition.Distance( toPosition ) / carrier.GetMaxSpeed() );
        return tools::DurationFromSeconds( duration );
    }
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView::Fill
// Created: SLI 2014-03-17
// -----------------------------------------------------------------------------
void DiagnosisUnitView::Fill( const std::vector< const kernel::Entity_ABC* >& destinations, const kernel::Agent_ABC& brokenAgent, const kernel::EquipmentType& carrier )
{
    Purge();
    int row = 0;
    for( auto it = destinations.begin(); it != destinations.end(); ++it )
    {
        QStandardItem* item = new QStandardItem();
        item->setData( gui::InternalLinks::CreateLink( "agent", (*it)->GetId(), (*it)->GetName() ), Qt::DisplayRole );
        item->setData( static_cast< unsigned int >( (*it)->GetId() ), gui::Roles::DataRole );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        model_->setItem( row, eName, item );
        item = new QStandardItem( ApproximateTimeTravel( **it, brokenAgent, carrier ) );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        model_->setItem( row, eTime, item );
        row++;
    }
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView::Purge
// Created: SLI 2014-03-19
// -----------------------------------------------------------------------------
void DiagnosisUnitView::Purge()
{
    model_->removeRows( 0, model_->rowCount() );
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView::Fill
// Created: SLI 2014-03-19
// -----------------------------------------------------------------------------
void DiagnosisUnitView::OnLinkClicked( const QString& url, const QModelIndex& /*index*/ )
{
    extractor_.NotifyLinkClicked( url + "#center" );
}

// -----------------------------------------------------------------------------
// Name: DiagnosisUnitView::OnSelectionChanged
// Created: SLI 2014-03-19
// -----------------------------------------------------------------------------
void DiagnosisUnitView::OnSelectionChanged( const QModelIndex& current, const QModelIndex& )
{
    const unsigned int destination = model_->data( current, gui::Roles::DataRole ).value< unsigned int >();
    emit DestinationSelected( destination );
}
