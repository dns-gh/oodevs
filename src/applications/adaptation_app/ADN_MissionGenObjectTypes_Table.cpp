// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionGenObjectTypes_Table.h"
#include "moc_ADN_MissionGenObjectTypes_Table.cpp"
#include "ADN_MissionGenObjectTypes_Infos.h"

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Table::ADN_MissionGenObjectTypes_Table( QCheckBox* all, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
    , all_( all )
    , isAdding_( false )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Type" ) << tr( "Allowed" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddCheckBoxOnColumn( 1 );
    proxyModel_->setDynamicSortFilter( true );
    Sort();
    setSortingEnabled( false );
    connect( all_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table destructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Table::~ADN_MissionGenObjectTypes_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::IsChecked
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
bool ADN_MissionGenObjectTypes_Table::IsChecked() const
{
    for( int nRow = 0; nRow < dataModel_.rowCount(); ++nRow )
    {
        const QModelIndex index = dataModel_.index( nRow, 1 );
        helpers::ADN_MissionGenObjectTypes_Infos* pInfos = static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( GetDataFromIndex( index ) );
        if( pInfos && !pInfos->isAllowed_.GetData() )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::doValueChanged
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );
    if( !isAdding_ )
    {
        bool wasBlocked = all_->blockSignals( true );
        all_->setChecked( IsChecked() );
        all_->blockSignals( wasBlocked );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::AddRow( int row, void* data )
{
    helpers::ADN_MissionGenObjectTypes_Infos* pInfos = static_cast< helpers::ADN_MissionGenObjectTypes_Infos* >( data );
    if( !pInfos )
        return;
    isAdding_ = true;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pInfos->isAllowed_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    isAdding_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Table::OnStateChanged
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Table::OnStateChanged( int state )
{
    if( dataModel_.rowCount() > 0 && dataModel_.columnCount() > 0)
        selectionModel()->setCurrentIndex( dataModel_.index( 0, 0 ), QItemSelectionModel::ClearAndSelect );
    for( int nRow = 0; nRow < dataModel_.rowCount(); ++nRow )
        if( QStandardItem* item = GetItemFromIndex( dataModel_.index( nRow, 1 ) ) )
            item->setCheckState( state == Qt::Checked ? Qt::Checked : Qt::Unchecked );
    doItemsLayout();
}
