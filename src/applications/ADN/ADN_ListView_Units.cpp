//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Units.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 11:48 $
// $Revision: 13 $
// $Workfile: ADN_ListView_Units.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_ListView_Units.h"

#include <qpopmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Unit_Wizard.h"
#include "ADN_Units_GUI.h"
#include "ADN_Automata_Data.h"

typedef ADN_Units_Data::UnitInfos UnitInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Units::ADN_ListView_Units( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Units" ) );
    setResizeMode( QListView::AllColumns );

    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<UnitInfos>( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Units::~ADN_ListView_Units()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Units::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    UnitInfos* pInfos = (UnitInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Units_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Units_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eTypeId]->Connect( &pInfos->eTypeId_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eModel]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eDecontaminationDelay]->Connect( &pInfos->decontaminationDelay_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eStrengthRatioFeedbackTime]->Connect( &pInfos->strengthRatioFeedbackTime_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eProbeWidth]->Connect( &pInfos->rProbeWidth_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eProbeLength]->Connect( &pInfos->rProbeLength_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureLevel]->Connect( &pInfos->eNatureLevel_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureWeapon]->Connect( &pInfos->eNatureWeapon_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureSpec]->Connect( &pInfos->eNatureSpec_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureQualifer]->Connect( &pInfos->eNatureQualifier_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureCategory]->Connect( &pInfos->eNatureCategory_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureMobility]->Connect( &pInfos->eNatureMobility_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNatureAtlas]->Connect( &pInfos->eNatureAtlas_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eMissionCapacity]->Connect( &pInfos->eMissionCapacity_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNbOfficer]->Connect( &pInfos->nNbOfficer_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eNbNCOfficer]->Connect( &pInfos->nNbNCOfficer_, bConnect );
    vItemConnectors_[ADN_Units_GUI::ePostures]->Connect( &pInfos->vPostures_, bConnect );
    vItemConnectors_[ADN_Units_GUI::ePointInfos]->Connect( &pInfos->vPointInfos_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eComposantes]->Connect( &pInfos->vComposantes_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eContenancesTC1]->Connect( &pInfos->contenancesTC1_.categories_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eStock]->Connect( &pInfos->stocks_.vLogThresholds_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eCanProbe]->Connect( &pInfos->bProbe_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eHasTC1]->Connect( &pInfos->bTC1_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eHasStock]->Connect( &pInfos->bStock_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eHasStrengthRatioFeedbackTime]->Connect( &pInfos->bStrengthRatioFeedbackTime_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eCanFly]->Connect( &pInfos->bCanFly_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eIsAutonomous]->Connect( &pInfos->bIsAutonomous_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eSensorsReach]->Connect( &pInfos->rSensorsReach_, bConnect );
    vItemConnectors_[ADN_Units_GUI::eWeaponsReach]->Connect( &pInfos->rWeaponsReach_, bConnect );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Units::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_Unit_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Units::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Units::GetToolTipFor( QListViewItem& item )
{
    void* pData = static_cast<ADN_ListViewItem&>( item ).GetData();
    UnitInfos* pCastData = (UnitInfos*)pData;
    assert( pCastData != 0 );

    std::string strToolTip = tr( "<b>Used by:</b><br>" ).ascii();
    strToolTip += ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData );

    return strToolTip;
}
