//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Crowds_FireEffectRoe_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 9 $
// $Workfile: ADN_Crowds_FireEffectRoe_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_FireEffectRoe_GUI.h"
#include "ADN_Tools.h"
#include "ADN_Crowds_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Crowds_Data::FireEffectRoeInfos FireEffectRoeInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffectRoe_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Crowds_FireEffectRoe_GUI::ADN_Crowds_FireEffectRoe_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Crowds_FireEffectRoe_GUI", "ROE" )
                      << tools::translate( "ADN_Crowds_FireEffectRoe_GUI", "Attrition Surface (m²)" )
                      << tools::translate( "ADN_Crowds_FireEffectRoe_GUI", "PH" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, INT_MAX );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0, 1 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffectRoe_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Crowds_FireEffectRoe_GUI::~ADN_Crowds_FireEffectRoe_GUI()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_FireEffectRoe_GUI::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Crowds_FireEffectRoe_GUI::AddRow( int row, void* data )
{
    FireEffectRoeInfos* info = static_cast< FireEffectRoeInfos* >( data );
    if( !info )
        return;
    AddItem( row, 0, data, QString( ENT_Tr::ConvertFromPopulationRoe( info->nRoe_, ENT_Tr_ABC::eToTr ).c_str() ), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &info->rAttritionSurface_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 2, data, &info->rPH_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
