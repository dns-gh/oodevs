//*****************************************************************************
//
// $Created: JDY 03-09-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Postures_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 9 $
// $Workfile: ADN_Equipement_Postures_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_Postures_GUI.h"
#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipement_Data::ModificatorPostureInfos ModificatorPostureInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipement_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_Postures_GUI::ADN_Equipement_Postures_GUI( const QString& objectName, const QString& strColCaption, ADN_Connector_ABC*& connector,  QWidget* pParent /*= 0*/ )
    : ADN_Table3( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << strColCaption
                      << tr( "PH factor" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.01, 2 );
    setMinimumHeight( int( ( eNbrUnitPosture + 1.2 ) * 20 ) );
    setShowGrid( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipement_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_Postures_GUI::~ADN_Equipement_Postures_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Postures_GUI::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Equipement_Postures_GUI::AddRow( int row, void* data )
{
    ModificatorPostureInfos* pPostureInfo = static_cast< ModificatorPostureInfos* >( data );
    if( !pPostureInfo )
        return;
    AddItem( row, 0, data, QString::fromStdString( ENT_Tr::ConvertFromUnitPosture( pPostureInfo->eType_, ENT_Tr_ABC::eToTr ) ), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pPostureInfo->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
