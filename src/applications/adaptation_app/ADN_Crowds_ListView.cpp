// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_ListView.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Crowds_Data::CrowdsInfos PopulationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_ListView::ADN_Crowds_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Crowds_ListView", ADN_Tr::ConvertFromWorkspaceElement( eCrowds ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView< PopulationInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_ListView::~ADN_Crowds_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Crowds_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    PopulationInfos* pInfos = static_cast< PopulationInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Crowds_GUI::eNbrGuiElements );
    vItemConnectors_[ ADN_Crowds_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eModel ]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eCrowdCollision ]->Connect( &pInfos->bCrowdCollision_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eConcentrationDensity ]->Connect( &pInfos->rConcentrationDensity_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eMoveDensity ]->Connect( &pInfos->rMoveDensity_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eMoveSpeed ]->Connect( &pInfos->rMoveSpeed_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eMale ]->Connect( &pInfos->repartition_.male_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eFemale ]->Connect( &pInfos->repartition_.female_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eChildren ]->Connect( &pInfos->repartition_.children_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eArmedIndividuals ]->Connect( &pInfos->armedIndividuals_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eDecontaminationDelay ]->Connect( &pInfos->decontaminationDelay_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eSpeeds ]->Connect( &pInfos->vSpeedInfos_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eHasSpeeds ]->Connect( &pInfos->bHasSpeeds_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eSpeedEffectAttitude ]->Connect( &pInfos->vSpeedEffectInfos_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eFireEffectAttitude ]->Connect( &pInfos->vFireEffectInfos_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eFireEffectRoe ]->Connect( &pInfos->vFireEffectRoeInfos_, bConnect );
    vItemConnectors_[ ADN_Crowds_GUI::eUrbanBlocDestructionAttitude ]->Connect( &pInfos->vUrbanEffectInfos_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Crowds_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< PopulationInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eCrowds ).c_str(), ADN_Workspace::GetWorkspace().GetCrowds().GetData().GetCrowds(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        PopulationInfos* pCastData = static_cast< PopulationInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetInhabitantsThatUse( *pCastData ), eInhabitants );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_ListView::GetToolTipFor
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
std::string ADN_Crowds_ListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    PopulationInfos* pCastData = static_cast< PopulationInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eInhabitants ).c_str(),
                            ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetInhabitantsThatUse( *pCastData ) );
}