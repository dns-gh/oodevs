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
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Workspace.h"
#include <qpopupmenu.h>

typedef ADN_Composantes_Data::SensorInfos SensorInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Composantes_Sensors_GUI_Connector
    : public ADN_Connector_Table_ABC
{
public:
    explicit ADN_Composantes_Sensors_GUI_Connector( ADN_Composantes_Sensors_GUI& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {
        // NOTHING
    }

    void AddSubItems( int i, void* obj )
    {
        assert(obj);
        ADN_TableItem_String* pItemCombo = 0;
        ADN_TableItem_Double* pItemInt = 0;
        // add a new row & set new values
        tab_.setItem( i, 0, pItemCombo = new ADN_TableItem_String( &tab_, obj, QTableItem::Never ) );
        tab_.setItem( i, 1, pItemInt = new ADN_TableItem_Double( &tab_, obj ) );
        // set table item properties
        pItemInt->GetValidator().setBottom( 0 );
        // connect items & datas
        pItemCombo->GetConnector().Connect( &static_cast< SensorInfos* >( obj )->ptrSensor_.GetData()->strName_ );
        pItemInt->GetConnector().Connect( &static_cast< SensorInfos* >( obj )->rHeight_ );
    }
private:
    ADN_Composantes_Sensors_GUI_Connector& operator=( const ADN_Composantes_Sensors_GUI_Connector& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Sensors_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Sensors_GUI::ADN_Composantes_Sensors_GUI( QWidget * parent )
    : ADN_Table2( parent, "ADN_Composantes_Sensors_GUI" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( QTable::Single );
    setShowGrid( false );
    setLeftMargin( 0 );
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Sensor" ) );
    horizontalHeader()->setLabel( 1, tr( "Height (m)" ) );
    // connector creation
    pConnector_ = new ADN_Composantes_Sensors_GUI_Connector( *this );
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
void ADN_Composantes_Sensors_GUI::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    std::auto_ptr< QPopupMenu > pTargetMenu( new QPopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData() != 0;
    ADN_Sensors_Data::T_SensorsInfos_Vector& vAllSensors = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();
    for( ADN_Sensors_Data::T_SensorsInfos_Vector::iterator it = vAllSensors.begin(); it != vAllSensors.end(); ++it )
    {
        bDisplayAdd = true;
        pTargetMenu->insertItem( ( *it )->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vAllSensors.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( *pTargetMenu );
    if( ! bDisplayAdd && !bDisplayRem )
        return;
    QPopupMenu* pMenu = new QPopupMenu( this );
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
    SensorInfos* pCurSensor = static_cast< SensorInfos* >( GetCurrentData() );
    if( pCurSensor )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurSensor );
    }
}
