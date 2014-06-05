// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Sensors_GUI.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Workspace.h"
#include <Qt3Support/q3popupmenu.h>

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::ADN_Equipments_Sensors_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    // peut etre selectionne & trie
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    setSelectionMode( QTableView::SingleSelection );
    setSortingEnabled( true );
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Equipments_Sensors_GUI", "Sensor" )
                      << tools::translate( "ADN_Equipments_Sensors_GUI", "Height (m)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, INT_MAX );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::~ADN_Equipments_Sensors_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI::GetWorkspaceInfos
// Created: LDC 2013-11-27
// -----------------------------------------------------------------------------
template<>
ADN_Type_Vector_ABC< ADN_Sensors_Data::SensorInfos >& ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::SensorInfos, ADN_Sensors_Data::SensorInfos >::GetWorkspaceInfos()
{
    return ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();
}

template<>
ADN_Type_Vector_ABC< ADN_Radars_Data::RadarInfos >& ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::RadarInfos, ADN_Radars_Data::RadarInfos >::GetWorkspaceInfos()
{
    return ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->GetRadars();
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
void ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::OnContextMenu( const QPoint& pt )
{
    std::unique_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetSelectedData() != 0;
    ADN_Type_Vector_ABC< SensorInfo >& vAllSensors = GetWorkspaceInfos();
    for( auto it = vAllSensors.begin(); it != vAllSensors.end(); ++it )
    {
        SensorInfo* pSensorInfos = *it;
        if( Contains( pSensorInfos ) )
            continue;
        bDisplayAdd = true;
        pTargetMenu->insertItem( pSensorInfos->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vAllSensors.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( *pTargetMenu );
    if( ! bDisplayAdd && !bDisplayRem )
        return;
    Q3PopupMenu* pMenu = new Q3PopupMenu( this );
    if( bDisplayAdd )
        pMenu->insertItem( tools::translate( "ADN_Equipments_Sensors_GUI", "Add sensor" ), pTargetMenu.get(), 0 );
    if( bDisplayRem )
        pMenu->insertItem( tools::translate( "ADN_Equipments_Sensors_GUI", "Remove sensor"), 1 );
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
// Name: ADN_Equipments_Sensors_GUI::Contains
// Created: JSR 2012-12-13
// -----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
bool ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::Contains( const SensorInfo* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        EquipmentInfo* infos = static_cast< EquipmentInfo* >( GetData( row, 0 ) );
        if( infos->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI::CreateNewSensor
/** @param  nSensor
*/
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
template<>
void ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::SensorInfos, ADN_Sensors_Data::SensorInfos >::CreateNewSensor( int nSensor )
{
    ADN_Equipments_Data::SensorInfos* pNewInfo = new ADN_Equipments_Data::SensorInfos();
    pNewInfo->SetCrossedElement( ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos()[ nSensor ] );
    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

template<>
void ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::RadarInfos, ADN_Radars_Data::RadarInfos >::CreateNewSensor( int nSensor )
{
    ADN_Equipments_Data::RadarInfos* pNewInfo = new ADN_Equipments_Data::RadarInfos();
    pNewInfo->SetCrossedElement( ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->GetRadars()[ nSensor ] );
    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI::RemoveCurrentSensor
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
void ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::RemoveCurrentSensor()
{
    EquipmentInfo* pCurSensor = static_cast< EquipmentInfo* >( GetSelectedData() );
    if( pCurSensor )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurSensor );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Sensors_GUI::AddRow
// Created: NPT 2012-11-06
// -----------------------------------------------------------------------------
template< typename EquipmentInfo, typename SensorInfo >
void ADN_Equipments_Sensors_GUI< EquipmentInfo, SensorInfo >::AddRow( int row, void* data )
{
    EquipmentInfo* infos = static_cast< EquipmentInfo* >( data );
    if( !infos )
        return;
    AddItem( row, 0, data, &infos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &infos->rHeight_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

// Explicit instanciation... because specialising some methods does not instantiate the constructors...
template class ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::SensorInfos, ADN_Sensors_Data::SensorInfos >;
template class ADN_Equipments_Sensors_GUI< ADN_Equipments_Data::RadarInfos, ADN_Radars_Data::RadarInfos >;