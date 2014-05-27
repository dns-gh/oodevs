// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_ModificatorTable.h"
#include "moc_ADN_Sensors_ModificatorTable.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_ModificatorTable_ABC constructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_ModificatorTable_ABC::ADN_Sensors_ModificatorTable_ABC( const QString& firstColumnName, const QString& secondColumnName, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent)
{
    //setSelectionBehavior( QAbstractItemView::SelectRows );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    setShowGrid( false );
    verticalHeader()->setVisible( false );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << firstColumnName << secondColumnName;
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.001, 3 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_ModificatorTable_ABC destructor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
ADN_Sensors_ModificatorTable_ABC::~ADN_Sensors_ModificatorTable_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_ModificatorTable_ABC::currentChanged
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_ModificatorTable_ABC::currentChanged( const QModelIndex& current, const QModelIndex& )
{
    InternalEmit( current );
}
