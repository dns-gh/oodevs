//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Postures_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 10 $
// $Workfile: ADN_Units_Postures_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Units_Postures_GUI.h"

#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Units_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Units_Data::PostureInfos PostureInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Units_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Postures_GUI::ADN_Units_Postures_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setFixedHeight( 110 );

    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Stance" )
                      << tr( "Time to activate" )
                      << tr( "Time to deactivate" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddDelayEditOnColumn( 1 );
    delegate_.AddDelayEditOnColumn( 2 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Postures_GUI::~ADN_Units_Postures_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Postures_GUI::AddRow
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Units_Postures_GUI::AddRow( int row, void* data )
{
    PostureInfos* pInfo = static_cast< PostureInfos* >( data );
    if( !pInfo )
        return;

    AddItem( row, 0, data, ENT_Tr::ConvertFromUnitPosture( pInfo->nPosture_, ENT_Tr_ABC::eToTr ).c_str() );
    AddItem( row, 1, data, &pInfo->timeToActivate_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pInfo->timeToDeactivate_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
}
