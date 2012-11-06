//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Sensors_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 11 $
// $Workfile: ADN_Composantes_Sensors_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Composantes_Sensors_GUI.h"
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Workspace.h"
#include <Qt3Support/q3popupmenu.h>

typedef ADN_Composantes_Data::SensorInfos SensorInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Sensors_GUI::ADN_Composantes_Sensors_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table3( objectName, connector, pParent )
{
    // peut etre selectionne & trie
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    setSelectionMode( QTableView::SingleSelection );
    setSortingEnabled( true );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Sensor" )
                      << tr( "Height (m)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddLineEditOnColumn( 0 );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, INT_MAX );
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Sensors_GUI::~ADN_Composantes_Sensors_GUI()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
void ADN_Composantes_Sensors_GUI::OnContextMenu( const QPoint& pt )
{
    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetSelectedData() != 0;
    ADN_Sensors_Data::T_SensorsInfos_Vector& vAllSensors = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();
    for( ADN_Sensors_Data::T_SensorsInfos_Vector::iterator it = vAllSensors.begin(); it != vAllSensors.end(); ++it )
    {
        bDisplayAdd = true;
        pTargetMenu->insertItem( ( *it )->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vAllSensors.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( *pTargetMenu );
    if( ! bDisplayAdd && !bDisplayRem )
        return;
    Q3PopupMenu* pMenu = new Q3PopupMenu( this );
    if( bDisplayAdd )
        pMenu->insertItem( tr( "Add sensor" ), pTargetMenu.get(), 0 );
    if( bDisplayRem )
        pMenu->insertItem( tr( "Remove sensor"), 1 );
    int nMenu = pMenu->exec( pt );
    if( nMenu == 1 )
        RemoveCurrentSensor();
    else if( nMenu > 1 )
    {
        assert( nMenu - 2 < static_cast< int >( vAllSensors.size() ) );
        CreateNewSensor( nMenu - 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI::CreateNewSensor
/** @param  nSensor
*/
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Composantes_Sensors_GUI::CreateNewSensor( int nSensor )
{
    SensorInfos* pNewInfo = new SensorInfos();
    pNewInfo->ptrSensor_ = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos()[ nSensor ];
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI::RemoveCurrentSensor
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Composantes_Sensors_GUI::RemoveCurrentSensor()
{
    SensorInfos* pCurSensor = static_cast< SensorInfos* >( GetSelectedData() );
    if( pCurSensor )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurSensor );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI::AddRow
// Created: NPT 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Composantes_Sensors_GUI::AddRow( int row, void* data )
{
    SensorInfos* infos = static_cast< SensorInfos* >( data );
    if( !infos )
        return;
    AddItem( row, 0, data, &infos->ptrSensor_.GetData()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &infos->rHeight_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
