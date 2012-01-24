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
ADN_Sensors_ModificatorTable_ABC::ADN_Sensors_ModificatorTable_ABC( QWidget* pParent, const char* szName, const QString& firstColumnName, const QString& secondColumnName )
    : ADN_Table2( pParent, szName )
    , lastCurrentRow_( 0 )
{
    setSorting(true);
    setSelectionMode( Q3Table::SingleRow );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    setShowGrid(false);
    setMaximumWidth( 300 );
    setLeftMargin(0);
    verticalHeader()->hide();

    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    horizontalHeader()->setLabel(0, firstColumnName );
    horizontalHeader()->setLabel(1, secondColumnName );

    connect( this, SIGNAL( selectionChanged() ), SLOT( OnCurrentChanged() ) );
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
// Name: ADN_Sensors_ModificatorTable_ABC::createEditor
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
QWidget* ADN_Sensors_ModificatorTable_ABC::createEditor( int row, int col, bool initFromCell ) const
{
    if( col == 0 )
        return 0;
    else
        return ADN_Table2::createEditor( row, col, initFromCell );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_ModificatorTable_ABC::paintCell
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_ModificatorTable_ABC::paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg )
{
    QColor backgroundColor;
    if( col == 0 )
        backgroundColor.setRgb( 230, 230, 230 );
    else
        backgroundColor.setRgb( 255, 255, 255 );

    QColorGroup newCg( cg );
    newCg.setColor( QColorGroup::Base, backgroundColor );
    ADN_Table2::paintCell( p, row, col, cr, selected, newCg );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_ModificatorTable_ABC::OnCurrentChanged
// Created: ABR 2012-01-16
// -----------------------------------------------------------------------------
void ADN_Sensors_ModificatorTable_ABC::OnCurrentChanged()
{
    if( isSelected( lastCurrentRow_, 0 ) )
    {
        clearSelection();
        lastCurrentRow_ = -1;
        emit ContentChanged( "", 1. ); // ADN_Sensors_DetectionAlgorithmPrevision default value
    }
    else
    {
        lastCurrentRow_ = currentRow();
        InternalEmit();
    }
    repaintContents();
}
