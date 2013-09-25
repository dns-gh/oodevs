// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionTypes_Table.h"
#include "ADN_Missions_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_MissionTypes_Table constructor
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_MissionTypes_Table::ADN_MissionTypes_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << qApp->translate( "ADN_Missions_GUI", "Type" ) << qApp->translate( "ADN_Missions_GUI", "Allowed" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddCheckBoxOnColumn( 1 );
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->sort( 0, Qt::AscendingOrder );
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionTypes_Table destructor
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_MissionTypes_Table::~ADN_MissionTypes_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionTypes_Table::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_MissionTypes_Table::AddRow( int row, void* data )
{
    ADN_Missions_Type* pInfos = static_cast< ADN_Missions_Type* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, pInfos->displayName_.c_str() );
    AddItem( row, 1, data, &pInfos->isAllowed_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
}
