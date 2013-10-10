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
#include "ADN_Missions_Wizard.h"
#include "ADN_enums.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Missions_Mission Mission;


// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_ListView_MissionTypes::ADN_ListView_MissionTypes( QWidget* parent, E_MissionType eMissionType, ADN_Missions_Data::T_Mission_Vector& missions )
    : ADN_ListView( parent, "ADN_ListView_MissionTypes", ADN_Tr::ConvertFromWorkspaceElement( eMissions ).c_str() )
    , missions_   ( missions )
    , eMissionType_( eMissionType )
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
    vItemConnectors_[ADN_Missions_GUI::eDescriptionSheetPath]->Connect( &pInfos->missionSheetPath_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionContext]->Connect( &pInfos->descriptionContext_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionParameters]->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionBehavior]->Connect( &pInfos->descriptionBehavior_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionSpecificCases]->Connect( &pInfos->descriptionSpecific_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionComments]->Connect( &pInfos->descriptionComment_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionMissionEnd]->Connect( &pInfos->descriptionMissionEnd_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionAttachments]->Connect( &pInfos->attachments_, bConnect );

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
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_MissionTypes::OnContextMenu
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_MissionTypes::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Missions_Wizard wizard( eMissionType_, ADN_Tr::ConvertFromWorkspaceElement( eMissions ).c_str(), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_ ), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        Mission* pCastData = static_cast< Mission* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eModels ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Tools::ConvertMissionToEntityType( eMissionType_ ), *pCastData ), eModels, static_cast< int >( eMissionType_ ) );
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
    boost::shared_ptr< kernel::LocalizedString > name = static_cast< ADN_Missions_ABC* >( pCurData_ )->strName_.GetTranslation();
    if( ADN_ListView::ContextMenuDelete() )
    {
        emit NotifyElementDeleted( name, eMissionType_ );
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
                            ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Tools::ConvertMissionToEntityType( eMissionType_ ), *pCastData ) );
}
