// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_MissionTypes.h"
#include "moc_ADN_ListView_MissionTypes.cpp"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Models_Data.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_enums.h"

typedef ADN_Missions_Mission Mission;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_ListView_MissionTypes::ADN_ListView_MissionTypes( E_EntityType eEntityType, ADN_Missions_Data::T_Mission_Vector& missions, QWidget* parent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_ListView( parent, szName, ADN_Tr::ConvertFromWorkspaceElement( eMissions ).c_str() )
    , missions_   ( missions )
    , eEntityType_( eEntityType )
{
    pConnector_ = new ADN_Connector_ListView< Mission >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_ListView_MissionTypes::~ADN_ListView_MissionTypes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::ConnectItem
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_MissionTypes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    Mission* pInfos = static_cast< Mission* >( pCurData_ );

    vItemConnectors_[ADN_Missions_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eMissionSheetDescription]->Connect( &pInfos->missionSheetContent_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]->Connect( &pInfos->diaType_, bConnect );
    if( vItemConnectors_[ADN_Missions_GUI::eBehavior] )
        vItemConnectors_[ADN_Missions_GUI::eBehavior]->Connect( &pInfos->diaBehavior_, bConnect );
    if( vItemConnectors_[ADN_Missions_GUI::eMRTBehavior] )
    {
        vItemConnectors_[ADN_Missions_GUI::eMRTBehavior]->Connect( &pInfos->mrtDiaBehavior_, bConnect );
        vItemConnectors_[ADN_Missions_GUI::eCDTBehavior]->Connect( &pInfos->cdtDiaBehavior_, bConnect );
    }
    vItemConnectors_[ ADN_Missions_GUI::eSymbol ]->Connect( &pInfos->symbol_, bConnect );

    emit SelectionChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::OnContextMenu
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_MissionTypes::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Missions_Data::T_Mission_Vector* pMissionList;
    if( eEntityType_ == eEntityType_Automat )
        pMissionList = &ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions();
    else if( eEntityType_ == eEntityType_Pawn )
        pMissionList = &ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions();
    else
        pMissionList = &ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions();

    ADN_Wizard< Mission > wizard( ADN_Tr::ConvertFromWorkspaceElement( eMissions ).c_str(), *pMissionList, this );
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
        FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        Mission* pCastData = static_cast< Mission* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eModels ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_, *pCastData ), eModels, static_cast< int >( eEntityType_ ) );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::ContextMenuDelete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
bool ADN_ListView_MissionTypes::ContextMenuDelete()
{
    if( pCurData_ == 0 )
        return false;
    std::string name = static_cast< Mission* >( pCurData_ )->strName_.GetData();
    if( ADN_ListView::ContextMenuDelete() )
    {
        emit NotifyMissionDeleted( name, eEntityType_ );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::GetToolTipFor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
std::string ADN_ListView_MissionTypes::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    Mission* pCastData = static_cast< Mission* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eModels ).c_str(),
                            ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_, *pCastData ) );
}
