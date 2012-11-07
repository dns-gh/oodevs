// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-10 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Automata_SubUnitsTable.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 12:13 $
// $Revision: 10 $
// $Workfile: ADN_Automata_SubUnitsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_SubUnitsTable.h"
#include "moc_ADN_Automata_SubUnitsTable.cpp"
#include "ADN_Automata_Data.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_MenuListView.h"

typedef ADN_Automata_Data::UnitInfos AutomatUnitInfos;
typedef ADN_Units_Data::UnitInfos    UnitInfos;

Q_DECLARE_METATYPE( UnitInfos* )

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Automata_SubUnitsTable::ADN_Automata_SubUnitsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table3( objectName, connector, pParent )
    , bMenuListItemSelected_( false )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Name" )
                      << tr( "Min Nbr" )
                      << tr( "Max Nbr" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 1 );
    delegate_.AddSpinBoxOnColumn( 2 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Automata_SubUnitsTable::~ADN_Automata_SubUnitsTable()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu& addMenu = *new Q3PopupMenu( &popupMenu );

    // Get the list of the possible units
    ADN_MenuListView< UnitInfos >* list = new ADN_MenuListView< UnitInfos >( this, ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), &addMenu );
    addMenu.addAction( list );

    popupMenu.insertItem( tr( "Add subordinate"), &addMenu );
    popupMenu.insertItem( tr( "Remove subordinate"), 0 );
    popupMenu.setItemEnabled( 0, GetSelectedData() != 0 );

    bMenuListItemSelected_ = false;
    int nMenuResult = popupMenu.exec(pt);
    if( nMenuResult == 0 )
        RemoveCurrentElement();
    else if( bMenuListItemSelected_ )
        AddNewElement( list ->SelectedValue() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddNewElement
// Created: APE 2005-05-09
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddNewElement( ADN_Units_Data::UnitInfos* info )
{
    if ( !info )
        return;
    AutomatUnitInfos* pNewInfo = new AutomatUnitInfos();
    pNewInfo->ptrUnit_ = info;
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::RemoveCurrentElement
// Created: APE 2005-05-09
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::RemoveCurrentElement()
{
    AutomatUnitInfos* pCurPh = static_cast< AutomatUnitInfos* >( GetSelectedData() );
    if( pCurPh != 0 )
    {
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurPh );
        emit ItemRemoved( pCurPh->ptrUnit_.GetData()->strName_.GetData() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::MenuListItemSelected
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::MenuListItemSelected()
{
    bMenuListItemSelected_ = true;
    qApp->exit_loop();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddSubItems
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddSubItems( const std::string& name )
{
    emit ItemAdded( name );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddRow( int row, void* data )
{
    AutomatUnitInfos* info = static_cast< AutomatUnitInfos* >( data );
    if( !info )
        return;
    AddItem( row, 0, data, &info->ptrUnit_.GetData()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &info->min_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &info->max_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddSubItems( info->ptrUnit_.GetData()->strName_.GetData() );
}
