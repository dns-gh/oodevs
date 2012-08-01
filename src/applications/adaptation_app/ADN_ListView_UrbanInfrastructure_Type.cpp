//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:45 $
// $Revision: 6 $
// $Workfile: ADN_ListView_Categories_Size.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_UrbanInfrastructure_Type.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type constructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanInfrastructure_Type::ADN_ListView_UrbanInfrastructure_Type(QWidget * parent, Qt::WFlags f)
:   ADN_ListView(parent,"Infrastructure",f)
{
    addColumn( tools::translate( "ADN_ListView_UrbanInfrastructure_Type", "Infrastructure" ) );
    setResizeMode( Q3ListView::LastColumn );
    pConnector_ = new ADN_Connector_ListView< ADN_Urban_Data::UrbanMaterialInfos >( *this );
    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type destructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanInfrastructure_Type::~ADN_ListView_UrbanInfrastructure_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type::ConnectItem
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void ADN_ListView_UrbanInfrastructure_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::InfrastructureInfos* pInfos = ( ADN_Urban_Data::InfrastructureInfos* )pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanInfrastructureGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanInfrastructureName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eUrbanInfrastructureSymbol]->Connect( &pInfos->pSymbol_, bConnect );

    ADN_Urban_Data::InfrastructureInfos::CIT_CapacityMap cit = pInfos->capacities_.find( "medical" );
    if( cit == pInfos->capacities_.end() )
        throw std::runtime_error( "unregister object capacity: medical ");

    ADN_Objects_Data::ADN_CapacityInfos_Medical* medical = static_cast< ADN_Objects_Data::ADN_CapacityInfos_Medical* >( cit->second.get() );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_NightRate]->Connect( &medical->emergencyBedsRate_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_EmergencyDoctorRate]->Connect( &medical->emergencyDoctorsRate_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_EmergencyBedRate]->Connect( &medical->nightDoctorsRate_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacityPresent]->Connect( &medical->bPresent_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type::OnContextMenu
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void  ADN_ListView_UrbanInfrastructure_Type::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Urban_Data::InfrastructureInfos > wizard( tools::translate( "ADN_ListView_UrbanInfrastructure_Type", "Infrastructures" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetInfrastructuresInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
