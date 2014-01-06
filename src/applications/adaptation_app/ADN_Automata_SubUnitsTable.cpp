// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_SubUnitsTable.h"
#include "moc_ADN_Automata_SubUnitsTable.cpp"
#include "ADN_Automata_Data.h"
#include "ADN_Gui_Tools.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Automata_Data::UnitInfos AutomatUnitInfos;
typedef ADN_Units_Data::UnitInfos    UnitInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Automata_SubUnitsTable::ADN_Automata_SubUnitsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Name" )
                      << tr( "Min Nbr" )
                      << tr( "Max Nbr" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
    delegate_.AddSpinBoxOnColumn( 2, -1, std::numeric_limits< int >::max() );
    proxyModel_->setDynamicSortFilter( true );
    Sort();
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
    ADN_Gui_Tools::GenerateStandardEditionDialog< UnitInfos, AutomatUnitInfos >(
        *this, pt, "subordinate-list", tr( "Subordinates" ), ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
        ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos() );
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
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurPh );
        emit ItemRemoved( pCurPh->GetCrossedElement()->strName_.GetData() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddRow( int row, void* data )
{
    AutomatUnitInfos* info = static_cast< AutomatUnitInfos* >( data );
    if( !info || !info->GetCrossedElement() )
        return;
    AddItem( row, 0, data, &info->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &info->min_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &info->max_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    emit ItemAdded( info->GetCrossedElement()->strName_.GetData() );
}
