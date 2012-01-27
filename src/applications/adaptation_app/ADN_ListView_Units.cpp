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

#include "adaptation_app_pch.h"
#include "ADN_ListView_Units.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Unit_Wizard.h"
#include "ADN_Units_GUI.h"
#include "ADN_Automata_Data.h"

typedef ADN_Units_Data::UnitInfos UnitInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Units::ADN_ListView_Units( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_ListView_Units", "Units" ) );

    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<UnitInfos>( *this );
    SetDeletionEnabled( true );
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
    UnitInfos* pInfos = static_cast< UnitInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Units_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_Units_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eTypeId ]->Connect( &pInfos->eTypeId_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eModel ]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eDecontaminationDelay ]->Connect( &pInfos->decontaminationDelay_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eStrengthRatioFeedbackTime ]->Connect( &pInfos->strengthRatioFeedbackTime_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eProbeWidth ]->Connect( &pInfos->rProbeWidth_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eProbeLength ]->Connect( &pInfos->rProbeLength_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eSensorRange ]->Connect( &pInfos->nSensorRange_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eEquipmentRange ]->Connect( &pInfos->nEquipmentRange_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eNatureLevel ]->Connect( &pInfos->eNatureLevel_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eNatureAtlas ]->Connect( &pInfos->eNatureAtlas_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eNatureNature ]->Connect( &pInfos->strNature_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eNbOfficer ]->Connect( &pInfos->nNbOfficer_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eNbNCOfficer ]->Connect( &pInfos->nNbNCOfficer_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::ePostures ]->Connect( &pInfos->vPostures_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::ePointInfos ]->Connect( &pInfos->vPointInfos_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eComposantes ]->Connect( &pInfos->vComposantes_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eContenancesTC1 ]->Connect( &pInfos->contenancesTC1_.categories_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eStock ]->Connect( &pInfos->stocks_.vLogThresholds_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCanProbe ]->Connect( &pInfos->bProbe_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eRanges ]->Connect( &pInfos->bRanges_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eHasTC1 ]->Connect( &pInfos->bTC1_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eHasStock ]->Connect( &pInfos->bStock_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eHasStrengthRatioFeedbackTime ]->Connect( &pInfos->bStrengthRatioFeedbackTime_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCanFly ]->Connect( &pInfos->bCanFly_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCrossingHeight ]->Connect( &pInfos->eCrossingHeight_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eIsAutonomous ]->Connect( &pInfos->bIsAutonomous_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eInstallationDelay ]->Connect( &pInfos->installationDelay_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eUninstallationDelay ]->Connect( &pInfos->uninstallationDelay_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eFootprintRadius ]->Connect( &pInfos->nFootprintRadius_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eSpeedModifier ]->Connect( &pInfos->rSpeedModifier_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eHasInstallation ]->Connect( &pInfos->bInstallationDelay_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eRecon ]->Connect( &pInfos->nReconEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCombatSupport ]->Connect( &pInfos->nCombatSupportEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCombat ]->Connect( &pInfos->nCombatEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eMobilitySupport ]->Connect( &pInfos->nMobilitySupportEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eCounterMobilitySupport ]->Connect( &pInfos->nCounterMobilitySupportEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eProtectionSupport ]->Connect( &pInfos->nProtectionSupportEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eEngineeringRecon ]->Connect( &pInfos->nEngineeringReconEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eUrbanAreaEfficiency ]->Connect( &pInfos->nUrbanAreaEfficiency_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eMalesPercent ]->Connect( &pInfos->repartition_.male_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eFemalesPercent ]->Connect( &pInfos->repartition_.female_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eChildrenPercent ]->Connect( &pInfos->repartition_.children_, bConnect );
    vItemConnectors_[ ADN_Units_GUI::eIsCivilian ]->Connect( &pInfos->bIsCivilian_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Units::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Units::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Unit_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        UnitInfos* pCastData = static_cast< UnitInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData ), eAutomata );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Units::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Units::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast< ADN_ListViewItem& >( item ).GetData();
    UnitInfos* pCastData = static_cast< UnitInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomataThatUse( *pCastData ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Units::ConnectNatureSymbol
// Created: MMC 2011-07-11
// -----------------------------------------------------------------------------
void ADN_ListView_Units::ConnectNatureSymbol( UnitInfos* pValidData )
{
    UnitInfos* pInfos = static_cast< UnitInfos* >( pValidData );
    if ( pInfos && ADN_Units_GUI::eNatureSymbol < vItemConnectors_.size() )
        vItemConnectors_[ ADN_Units_GUI::eNatureSymbol ]->Connect( &pInfos->natureSymbol_, true );
}