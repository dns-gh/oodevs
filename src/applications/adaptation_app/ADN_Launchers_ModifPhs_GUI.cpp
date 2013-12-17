//*****************************************************************************
//
// $Created: JDY 03-09-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_ModifPhs_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 8 $
// $Workfile: ADN_Launchers_ModifPhs_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Launchers_ModifPhs_GUI.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Launchers_ModifPhs_GUI::ADN_Launchers_ModifPhs_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    :   ADN_Table( objectName, connector, pParent )
{
    setSelectionMode( QAbstractItemView::NoSelection );
    setShowGrid( false );

    dataModel_.setColumnCount( eNbrUnitPosture );
    dataModel_.setRowCount( eNbrUnitPosture );
    QStringList headers;
    for( int i = 0; i < eNbrUnitPosture; ++i )
    {
        headers.append( ENT_Tr::ConvertFromUnitPosture( static_cast< E_UnitPosture >( i ), ENT_Tr::eToTr).c_str() );
        delegate_.AddColorOnColumn( i, 0., 100. );
        delegate_.AddDoubleSpinBoxOnColumn( i, 0, 100, 1, 5 );
    }
    dataModel_.setHorizontalHeaderLabels( headers );
    dataModel_.setVerticalHeaderLabels( headers );

    horizontalHeader()->setResizeMode( QHeaderView::Stretch );

    setMaximumHeight( ( eNbrUnitPosture + 1 ) * rowHeight( 0 ) + 10 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Launchers_ModifPhs_GUI::~ADN_Launchers_ModifPhs_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI::AddRow
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Launchers_ModifPhs_GUI::AddRow( int row, void* data )
{
    ADN_Type_Line_ABC< ADN_Launchers_Data::ModifPhsInfos >* pLine = static_cast< ADN_Type_Line_ABC< ADN_Launchers_Data::ModifPhsInfos >* >( data );
    if( !pLine )
        return;

    QStringList headers;
    for( int col = 0; col < eNbrUnitPosture; ++col )
    {
        AddItem( row, col, data, pLine->operator[]( col ), ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        headers.append( ENT_Tr::ConvertFromUnitPosture( static_cast< E_UnitPosture >( col ), ENT_Tr::eToTr).c_str() );
    }
    dataModel_.setVerticalHeaderLabels( headers );
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI::dataChanged
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Launchers_ModifPhs_GUI::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );
    if( topLeft == bottomRight )
        ADN_Workspace::GetWorkspace().GetWeapons().GetGui().UpdateModifiers();
}
