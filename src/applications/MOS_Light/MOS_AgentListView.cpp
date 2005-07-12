//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListView.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 11:55 $
// $Revision: 174 $
// $Workfile: MOS_AgentListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_AgentListView.h"
#include "moc_MOS_AgentListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentListView.inl"
#endif

#include <QHeader.h>
#include <QAction.h>
#include <QWidgetStack.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_AgentServerConnectionMgr.h"

#include "MOS_PointListViewItem.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_AgentListViewItem_ABC.h"
#include "MOS_CheckListItem_ABC.h"
#include "MOS_ArmeListViewItem.h"
#include "MOS_AgentCheckListItem_ABC.h"
#include "MOS_AutomateCheckListItem.h"
#include "MOS_AutomateListViewItem.h"
#include "MOS_PionCheckListItem.h"
#include "MOS_PionListViewItem.h"
#include "MOS_AgentSelector.h"
#include "MOS_AgentManager.h"
#include "MOS_KnowledgeGroupListViewItem.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"


#include "MOS_AttrEditor.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_AgentModel.h"

// Missions
#include "MOS_MissionAction.h"
#include "MOS_Mission_ABC.h"
#include "MOS_MissionPion.h"
#include "MOS_MissionAutomate.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentMission.h"
#include "MOS_Mission_OC.h"
#include "MOS_AgentTest.h"
#include "MOS_MOSServer.h"

#include "MOS_ASN_Messages.h"

#include <qdragobject.h>

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView constructor
// Created: FBD 02-12-17
// Last modified: JVT 03-04-08
//-----------------------------------------------------------------------------
MOS_AgentListView::MOS_AgentListView( QWidget* pParent, E_ListViewType nType )
    :   QListView       ( pParent )
    ,   MOS_GLMouse_ABC ()
    ,   nType_          ( nType )
    ,   pSelectedAgent_ ( 0 )
    ,   pAgent_         ( 0 )
    ,   bFollow_        ( false )
    ,   bItineraire_    ( true )
    ,   bAllItineraire_ ( false )
    ,   bDrawAltitude_  ( true )
    ,   bAllLima_       ( false )
    ,   bAllLimit_      ( false )
    ,   bAllVision_     ( true )
    ,   bOptiDraw_      ( true )
    ,   bAllCone_       ( false )
    ,   pMissionTest_   ( 0 )
    ,   bNotLock_       ( false )
    ,   bPointEdition_  ( false )
    ,   bDrawOldPath_   ( false )
    ,   bDrawText_      ( false )
    ,   bScrollTrace_   ( true )
{
    setAcceptDrops( TRUE );
    setSorting( -1 );
    header()->hide();
    addColumn( "Agents" );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( selectionChanged( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );

    setRootIsDecorated( false );

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentListView::~MOS_AgentListView()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::RegisterMission
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
void MOS_AgentListView::RegisterMission( E_MissionID nTypeMission, E_TypeAgent nTypeAgent )
{
    QWidgetStack* pStack = MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft();

    IT_MissionDialogPtrMap itDialog = missionDialogPtrMap_.find( nTypeMission );
    if( itDialog != missionDialogPtrMap_.end() )
        return;

    MOS_Mission_ABC* pNewMission = 0;
    switch( nTypeAgent )
    {
        case eTypeAgentPion:            pNewMission = new MOS_MissionPion    ( nTypeMission, pStack ); break;
        case eTypeAgentAutomateDebraye: pNewMission = new MOS_MissionPion    ( nTypeMission, pStack ); break;
        case eTypeAgentAutomateEmbraye: pNewMission = new MOS_MissionAutomate( nTypeMission, pStack ); break;
    }
    missionDialogPtrMap_.insert( std::make_pair( nTypeMission, pNewMission ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::RegisterConduiteOrder
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
void MOS_AgentListView::RegisterConduiteOrder( E_OrderConduiteID nTypeMission )
{
    QWidgetStack* pStack = MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft();

    IT_OCDialogAgentTypePtrMap itDialog = orderConduiteDialogPtrMap_.find( nTypeMission );
    assert( itDialog == orderConduiteDialogPtrMap_.end() );

    MOS_Mission_ABC* pMission = new MOS_Mission_OC( nTypeMission, pStack );
    orderConduiteDialogPtrMap_.insert( std::make_pair( nTypeMission, pMission ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetConduiteOrder
// Created: AGN 03-04-14
//-----------------------------------------------------------------------------
MOS_Mission_ABC* MOS_AgentListView::GetConduiteOrder( E_OrderConduiteID nTypeMission )
{
    IT_OCDialogAgentTypePtrMap foundMap2 = orderConduiteDialogPtrMap_.find( nTypeMission );
    if( foundMap2 == orderConduiteDialogPtrMap_.end() )
        return 0;
    else
        return foundMap2->second;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentListView::Initialize()
{
    if( nType_ != eListViewMission )
    {
//        CreateArme( eTypeArmeNBC );
//        CreateArme( eTypeArmeRens );
//        CreateArme( eTypeArme3D );
//        CreateArme( eTypeArmeAlat );
//        CreateArme( eTypeArmeInfanterie );
//        CreateArme( eTypeArmeBlinde );
//        CreateArme( eTypeArmeGenie );
//        CreateArme( eTypeArmeLog );
        CreateArme( eTypeArmeUnknown );
  
        RegisterConduiteOrder( eOrdreConduite_ChangerReglesEngagement               );
        RegisterConduiteOrder( eOrdreConduite_Pion_AppliquerFeux                    );
        RegisterConduiteOrder( eOrdreConduite_Pion_RenforcerEnVSRAM                 );
        RegisterConduiteOrder( eOrdreConduite_Pion_TransfererVSRAM                  );
        RegisterConduiteOrder( eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM          );
        RegisterConduiteOrder( eOrdreConduite_ModifierTempsBordeeMaintenance        );
        RegisterConduiteOrder( eOrdreConduite_ModifierTempsBordeeSante              );
        RegisterConduiteOrder( eOrdreConduite_ModifierPrioritesReparations          );
        RegisterConduiteOrder( eOrdreConduite_ModifierPrioritesBlesses              );
        RegisterConduiteOrder( eOrdreConduite_ModifierPrioritesTactiquesBlesses     );
        RegisterConduiteOrder( eOrdreConduite_ModifierPrioritesTactiquesReparations );
        RegisterConduiteOrder( eOrdreConduite_Pion_ChangerDePosition                );
        RegisterConduiteOrder( eOrdreConduite_AcquerirObjectif                      );
        RegisterConduiteOrder( eOrdreConduite_AssignerPositionEmbarquement          );
        RegisterConduiteOrder( eOrdreConduite_Automate_RealiserVariantement         );
        RegisterConduiteOrder( eOrdreConduite_Automate_DesequiperSiteFranchissement );
        RegisterConduiteOrder( eOrdreConduite_Automate_ReagirFaceAEni               );
        RegisterConduiteOrder( eOrdreConduite_Automate_AffecterPionAObstacle        );
        RegisterConduiteOrder( eOrdreConduite_Automate_ReconnaitreZoneImplantation  );     
        RegisterConduiteOrder( eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement  );     
        RegisterConduiteOrder( eOrdreConduite_Automate_ActiverObstacle  );     
        RegisterConduiteOrder( eOrdreConduite_Automate_TransfererRenforts );     
    }
    MOS_App::GetApp().GetAgentManager().RegisterAgentListView( *this );
    MOS_GLMouse_ABC::Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
    MOS_App::GetApp().GetAgentManager().UnregisterAgentListView( *this );

    if( nType_ != eListViewMission )
    {
//        DeleteArme( eTypeArmeInfanterie );
//        DeleteArme( eTypeArmeBlinde );
//        DeleteArme( eTypeArmeGenie );
//        DeleteArme( eTypeArme3D );
//        DeleteArme( eTypeArmeAlat );
//        DeleteArme( eTypeArmeLog );
//        DeleteArme( eTypeArmeRens );
//        DeleteArme( eTypeArmeNBC );
        DeleteArme( eTypeArmeUnknown );


        for( IT_MissionDialogPtrMap it = missionDialogPtrMap_.begin(); it != missionDialogPtrMap_.end(); ++it )
            delete it->second;

        missionDialogPtrMap_.clear();
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::CreateAgent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem* MOS_AgentListView::CreateAgent( MOS_Agent& agent )
{

    if( nType_ == eListViewSelector )
    {
        MOS_AgentCheckListItem_ABC* pAgent = 0;
        if( agent.IsAutomate() )
        {
            MOS_ArmeListViewItem& arme = GetArme( agent.GetArme() );
            pAgent = (MOS_AgentCheckListItem_ABC*)&arme.CreateAutomate( agent, this, true );

        }
        else
        {
            MOS_AutomateCheckListItem* pAutomateParent = (MOS_AutomateCheckListItem*)agent.GetParent()->GetItem( *this );
            if( pAutomateParent )
                pAgent = (MOS_AgentCheckListItem_ABC*)&pAutomateParent->CreatePion( agent );
        }
        if( pAgent )
            RegisterAgent( *pAgent );

        assert( pAgent );
        return pAgent;

    }
    else
    if( nType_ == eListViewEditor )
    {
        MOS_AgentListViewItem_ABC* pAgent = 0;
        if( agent.IsAutomate() )
        {
            MOS_ArmeListViewItem& arme = GetArme( agent.GetArme() );
            pAgent = (MOS_AgentListViewItem_ABC*)&arme.CreateAutomate( agent, this );
        }
        else
        {
            MOS_AutomateListViewItem* pAutomateParent = (MOS_AutomateListViewItem*)agent.GetParent()->GetItem( *this );
            if( pAutomateParent )
                pAgent = (MOS_AgentListViewItem_ABC*)&pAutomateParent->CreatePion( agent );
        }
        if( pAgent )
            RegisterAgent( *pAgent );

        assert( pAgent );
        return pAgent;
    }
    if( nType_ == eListViewMission )
    {
        return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::DeleteAgent
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_AgentListView::DeleteAgent( QListViewItem& agent )
{
    delete &agent;
    UnregisterAgent( agent );
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::RegisterAgent
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_AgentListView::RegisterAgent( QListViewItem& agent )
{
    agentVector_.push_back( &agent );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::UnregisterAgent
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_AgentListView::UnregisterAgent( QListViewItem& agent )
{
    IT_AgentPtrVector itAgent = std::find( agentVector_.begin(), agentVector_.end(), &agent );
    assert( itAgent != agentVector_.end() );
    agentVector_.erase( itAgent );
}


//=============================================================================
// SLOTS
//=============================================================================




//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GiveThisMission
// Created: FBD 03-09-15
//-----------------------------------------------------------------------------
/*void MOS_AgentListView::GiveThisMission( MOS_AgentTest& agentTest, MT_OutputArchive_ABC& archive, MOS_Mission_ABC& mission )
{
    MOS_Agent& agent = agentTest.GetAgent();

    if(    agent.GetModel() == 0
        || ! agent.GetModel()->CanExecuteMission( mission.GetTypeMission() ) )
        return;

    archive.WriteField( "sName", std::string( "GiveAMission|nMission" ) );
    archive.WriteField( "nMission", mission.GetTypeMission() );
    MT_LOG_INFO_MSG( MT_FormatString( "Agent[%d] Mission[%s] ", agent.GetAgentID(), mission.GetName().c_str() ).c_str() );
    mission.ResaveParameters( archive );
}
*/


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GiveAMission
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
/*
void MOS_AgentListView::GiveAMission( MOS_AgentTest& agentTest, MT_OutputArchive_ABC& archive )
{
    MOS_Agent& agent = agentTest.GetAgent();
    if(    agent.GetModel() == 0
        || agent.IsAutomate() )
        return;


    const MOS_AgentModel::T_Mission_Vector& vAvailableMissions = agent.GetModel()->GetAvailableMissions();
    if( vAvailableMissions.empty() )
        return;

    uint nMaxMission = vAvailableMissions.size();
    uint nMission = rand() % nMaxMission;

    MOS_Mission_ABC& mission = *(missionDialogPtrMap_[vAvailableMissions[nMission]]);

    archive.WriteField( "sName", std::string( "GiveAMission|nMission" ) );
    archive.WriteField( "nMission", mission.GetTypeMission() );
    MT_LOG_INFO_MSG( MT_FormatString( "Agent[%d] Mission[%s] ", agent.GetAgentID(), mission.GetName().c_str() ).c_str() );

    mission.Initialize();
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( &mission );
    mission.SetAgent( &agent );
    mission.FillRandomParameters();
    mission.ResaveParameters( archive );

    mission.Validate();
}
*/


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::ReplayAMission
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
/*
void MOS_AgentListView::ReplayAMission( MOS_AgentTest& agentTest, MT_InputArchive_ABC& archive )
{
    MOS_Agent& agent = agentTest.GetAgent();

    if(    agent.GetModel() == 0
        || agent.IsAutomate() )
        return;


    std::string sName;
    archive.ReadField( "sName", sName );
    assert( sName == "GiveAMission|nMission" );
    int nMission = 0;
    archive.ReadField( "nMission", nMission );


    MOS_Mission_ABC& mission = *(missionDialogPtrMap_[(E_MissionID)nMission]);
    MT_LOG_INFO_MSG( MT_FormatString( "Agent[%d] Mission[%s] ", agent.GetAgentID(), mission.GetName().c_str() ).c_str() );

    mission.Initialize();
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( &mission );
    mission.SetAgent( &agent );
    mission.ReloadParameters( archive );

    mission.Validate();
}
*/

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotTestMission
// Created: FBD 03-02-06
//-----------------------------------------------------------------------------
/*void MOS_AgentListView::SlotTestMission()
{
    if( pSelectedAgent_ == 0)
        return;

    MOS_Agent& agent = ((MOS_AgentListViewItem_ABC*)pSelectedAgent_)->GetAgent();

    if(    agent.GetModel() == 0
        || agent.IsAutomate() )
        return;

    pMissionTest_ = 0;

    const MOS_AgentModel::T_Mission_Vector& vAvailableMissions = agent.GetModel()->GetAvailableMissions();
    for( MOS_AgentModel::CIT_Mission_Vector itMission = vAvailableMissions.begin(); itMission != vAvailableMissions.end(); ++itMission )
    {
        E_MissionID nMission = (*itMission);

        IT_MissionDialogPtrMap foundMission = missionDialogPtrMap_.find( nMission );
        if( foundMission == missionDialogPtrMap_.end() )
        {
            QWidgetStack* pStack = MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft();
            MOS_Mission_ABC* pNewMission = 0;
            if( !agent.IsAutomate() )
                pNewMission = new MOS_MissionPion    ( nMission, pStack ); 
            else if( agent.IsEmbraye() )
                pNewMission = new MOS_MissionPion    ( nMission, pStack ); 
            else
                pNewMission = new MOS_MissionAutomate( nMission, pStack ); 
            foundMission = missionDialogPtrMap_.insert( std::make_pair( nMission, pNewMission ) ).first;
        }

            

        MOS_Mission_ABC& mission = *(foundMission->second);

        mission.Initialize();
        printf( "Start mission : %s\n", mission.GetName().c_str() );

        MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( &mission );
        mission.SetAgent( &agent );
        mission.Validate();
        pMissionTest_ = &mission;
        nStateTest_ = 0;
        while( nStateTest_ == 0 )
        {
            MOS_App::GetApp().GetMOSServer().Update();
            MOS_App::GetApp().processEvents();
        }
        printf( "End mission : %s\n", mission.GetName().c_str() );
        if( nStateTest_ == 2)
            break;
    }

    pMissionTest_ = 0;

}
*/

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetFollow
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetFollow( bool bFollow  )
{
    bFollow_ = bFollow;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetPointEdition
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetPointEdition( bool bValue )
{
    bPointEdition_ = bValue;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetItineraire
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetItineraire( bool bItineraire  )
{
    bItineraire_ = bItineraire;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetAllItineraire
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetAllItineraire( bool bAllItineraire  )
{
    bAllItineraire_ = bAllItineraire;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetDrawAltitude
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetDrawAltitude( bool bDrawAltitude  )
{
    bDrawAltitude_ = bDrawAltitude;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetOptiDraw
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetOptiDraw( bool bOptiDraw  )
{
    bOptiDraw_ = bOptiDraw;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetAllVision
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetAllVision( bool bAllVision  )
{
    bAllVision_ = bAllVision;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetAllCone
// Created: JVT 03-04-08
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetAllCone( bool bAllCone )
{
    // $$$$ AGE 2005-03-11: n'importe quoi
    if( ! MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return;
    //$$$ A DEPLACER ...
    bAllCone_ = bAllCone;

    if( bAllCone )
        MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgEnableUnitVisionCones();
    else
        MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgDisableUnitVisionCones();

    for( CIT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end() ; ++itAgent )
        ( (MOS_AgentListViewItem_ABC*)*itAgent )->GetAgent().bNeedRedrawVision_ = true;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotEnableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void MOS_AgentListView::SlotEnableProfiling( bool bEnable )
{
    // $$$$ AGE 2005-03-11: n'importe quoi
    if( ! MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return;
    //$$$ A DEPLACER ...

    if( bEnable )
        MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgEnableProfiling();
    else
        MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgDisableProfiling();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    if( nType_ == eListViewEditor )
    {
        OpenPopupMission( pItem );
    }
    else
    if( nType_ == eListViewMission )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem )
        {
            pMenu->insertItem( QString( "Add Agent"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if( nSelected == -1)
            return;

        if( nSelected == 0 )
        {
            MOS_AgentListView* pListView = &MOS_App::GetApp().GetMainWindow().GetAgentSelector();
            MOS_AgentSelector* pSelector = (MOS_AgentSelector*)pListView->parent();
            pSelector->SetAgentMission( *this );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pSelector );
        }
    }
    else
    if( nType_ == eListViewSelector )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem )
        {
//            pMenu->insertItem( QString( "Edit LIMA"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if( nSelected == -1)
            return;
    }


}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotSetDrawOldPath
// Created: APE 04-02-03
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotSetDrawOldPath( bool bDrawOldPath )
{
    bDrawOldPath_ = bDrawOldPath;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::OpenPopupMission
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_AgentListView::OpenPopupMission( QListViewItem* pItem )
{
    QPopupMenu* pMenu = 0;
    if( pItem )
    {
        MOS_ListViewItem_ABC* pItemAgent = (MOS_ListViewItem_ABC*)pItem;
        if( pItemAgent->GetItemType() == eItemTypeAutomate  || pItemAgent->GetItemType() == eItemTypePion )
        {
            MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)pItemAgent;
            pMenu = GetPopupMenu( pAgent->GetAgent() );
        }

        if( pMenu && !pMenu->isVisible() )
        {
//            pMenu->move( QCursor::pos() );
            pMenu->exec( QCursor::pos() );
        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::LocOnAgent
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_AgentListView::LocOnAgent( QListViewItem* pItem, bool bSelectList )
{
    if( pItem )
    {
        if( bSelectList )
            setSelected( pItem, true );

        if( nType_ == eListViewSelector )
        {
            MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)pItem;
            MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pAgent->GetAgent().GetPos() );
        }
        else
        {
            MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)pItem;
            MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pAgent->GetAgent().GetPos() );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_AgentListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem )
    {
        if( nType_ == eListViewEditor )
        {
            MOS_App::GetApp().GetMainWindow().GetAttributeEditor().SetAgent( 0 );
            pSelectedAgent_ = 0;
        }
        return;
    }

    E_ItemType nType = eItemTypeNone;
    if( nType_ == eListViewSelector )
    {
        MOS_CheckListItem_ABC* pTmpItem = (MOS_CheckListItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }
    else
    {
        MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }

    // Agent
    if( nType == eItemTypeAutomate || nType == eItemTypePion )
    {
        pSelectedAgent_ = pItem;
        if( nType_ == eListViewEditor )
        {
            if( pSelectedAgent_ )
            {
                if( nType_ == eListViewSelector )
                {
                    MOS_AgentCheckListItem_ABC* pTmpItem = (MOS_AgentCheckListItem_ABC*)pItem;
                    MOS_App::GetApp().GetMainWindow().GetAttributeEditor().SetAgent( & pTmpItem->GetAgent() );
                }
                else
                {
                    MOS_AgentListViewItem_ABC* pTmpItem = (MOS_AgentListViewItem_ABC*)pItem;
                    MOS_App::GetApp().GetMainWindow().GetAttributeEditor().SetAgent( & pTmpItem->GetAgent() );
                }
            }
        }
        if( !bNotLock_ )
            LocOnAgent( pSelectedAgent_, false );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::CreateArme
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem* MOS_AgentListView::CreateArme( E_TypeArme nTypeArme )
{
    MOS_ArmeListViewItem* pArme = new MOS_ArmeListViewItem( *this, BuildNameArme( nTypeArme ) );
    armePtrMap_.insert( std::make_pair( nTypeArme, pArme ) );
    return pArme;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::DeleteArme
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
void MOS_AgentListView::DeleteArme( E_TypeArme nTypeArme )
{
    IT_ArmePtrMap itArme = armePtrMap_.find( nTypeArme );
    assert( itArme != armePtrMap_.end() );
    delete itArme->second;
    armePtrMap_.erase( itArme );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetArme
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem& MOS_AgentListView::GetArme( E_TypeArme nTypeArme )
{
    IT_ArmePtrMap itArme = armePtrMap_.find( nTypeArme );
    assert( itArme != armePtrMap_.end() );
    MOS_ArmeListViewItem* pArme = itArme->second;
    return *pArme;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetPopupMenu
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
QPopupMenu* MOS_AgentListView::GetPopupMenu( const MOS_Agent& agent )
{
    const MOS_AgentModel* pModelPion     = agent.GetModelPion();
    const MOS_AgentModel* pModelAutomate = agent.GetModelAutomate();
    const MOS_AgentModel* pModel = pModelPion;

    MOS_Agent* pParent = agent.GetParent();

    if( pModel == 0 )
        return 0;

    QPopupMenu* pMenu = new QPopupMenu( this );

    if( agent.IsAutomate() && agent.IsEmbraye() )
        pModel = pModelAutomate;

    assert( pModel );

    const MOS_AgentModel::T_Mission_Vector& vAvaibleMissions = pModel->GetAvailableMissions();

    for( MOS_AgentModel::CIT_Mission_Vector itMission = vAvaibleMissions.begin(); itMission != vAvaibleMissions.end(); ++itMission )
    {
        E_MissionID nMission = (*itMission);

        CIT_MissionDialogPtrMap foundMission = missionDialogPtrMap_.find( nMission );
        if( foundMission == missionDialogPtrMap_.end() )
        {
            QWidgetStack* pStack = MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft();
            MOS_Mission_ABC* pNewMission = 0;
            if( pModel == pModelAutomate )
                pNewMission = new MOS_MissionAutomate( nMission, pStack ); 
            else 
                pNewMission = new MOS_MissionPion( nMission, pStack ); 
                
            foundMission = missionDialogPtrMap_.insert( std::make_pair( nMission, pNewMission ) ).first;
        }


        MOS_MissionAction* pAction = foundMission->second->GetOrCreateAction( *this );

        bool bEnabled = true;
        if( agent.IsAutomate() )
        {
//            if( agent.IsEmbraye() && foundMission->second->IsAutomateMission() )
//            bEnabled = true;
//            if( ! agent.IsEmbraye() && ! foundMission->second->IsAutomateMission() )
//            bEnabled = true;
        }
        else if( pParent != 0 && pParent->IsAutomate() && ! pParent->IsEmbraye() )
            bEnabled = true;
        
        pAction->setEnabled( bEnabled );
        pAction->addTo( pMenu );
    }

    return pMenu;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
std::string MOS_AgentListView::BuildNameArme( E_TypeArme nTypeArme )
{
    std::string sName( "" );
    switch( nTypeArme )
    {
//        case eTypeArme3D         : sName = "3D";           break;
//    case eTypeArmeBlinde     : sName = "Blinde";       break;
//    case eTypeArmeGenie      : sName = "Genie";        break;
//    case eTypeArmeInfanterie : sName = "Infanterie";   break;
//    case eTypeArmeAlat       : sName = "Alat";         break;
//    case eTypeArmeLog        : sName = "Logistique";   break;
//    case eTypeArmeRens       : sName = "Rens";         break;
//    case eTypeArmeNBC        : sName = "NBC";          break;
    case eTypeArmeUnknown : sName = "ODB"; break;
        default:
            assert( false );
    }
    return sName;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetSelectedAgent
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
MOS_Agent* MOS_AgentListView::GetSelectedAgent() const
{
    if( pSelectedAgent_ == 0 )
        return 0;

    if( nType_ == eListViewSelector )
    {
        MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)pSelectedAgent_;
        return &pAgent->GetAgent();
    }
    else
    {
        MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)pSelectedAgent_;
        return &pAgent->GetAgent();
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AgentListView::LMBPressEvent( const MT_Vector2D& vPos )
{
    bool bReturn = false;

    QListViewItem* pSelectedAgent = 0;

    for( CIT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end(); ++itAgent )
    {
        if( nType_ == eListViewSelector )
        {
            MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)*itAgent;
            if( pAgent->GetAgent().IsInside( vPos ) )
                pSelectedAgent = pAgent;
        }
        else
        {
            MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)( *itAgent );
            if( pAgent->GetAgent().IsInside( vPos ) )
                pSelectedAgent = pAgent;
        }
    }

    if( pSelectedAgent )
    {
        pSelectedAgent_ = pSelectedAgent;
        pSelectedAgent_->parent()->setOpen(true);
        if( pSelectedAgent_->parent()->parent() )
            pSelectedAgent_->parent()->parent()->setOpen( true );

        bNotLock_ = true;
        setSelected( pSelectedAgent_, true );
        bNotLock_ = false;

        bReturn = true;
    }
    return bReturn;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_AgentListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    if( bPointEdition_ )
    {
        MOS_App::GetApp().SavePoint( vPos );
        return false;
    }

    bool bReturn = false;

    QListViewItem* pSelectedAgent = 0;

    for( CIT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end(); ++itAgent )
    {
        if( nType_ == eListViewSelector )
        {
            MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)*itAgent;
            if( pAgent->GetAgent().IsInside( vPos ) )
                pSelectedAgent = pAgent;
        }
        else
        {
            MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)( *itAgent );
            if( pAgent->GetAgent().IsInside( vPos ) )
                pSelectedAgent = pAgent;
        }
    }

    if( pSelectedAgent )
    {
        pSelectedAgent_ = pSelectedAgent;
        pSelectedAgent_->parent()->setOpen(true);
        if( pSelectedAgent_->parent()->parent() )
            pSelectedAgent_->parent()->parent()->setOpen( true );

        bNotLock_ = true;
        setSelected( pSelectedAgent_, true );
        OpenPopupMission( pSelectedAgent_ );
        bNotLock_ = false;

        bReturn = true;
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentListView::MouseMoveEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_AgentListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AgentListView::IsActive( bool bMove )
{
    if( nType_ == eListViewSelector )
    {
        MOS_AgentSelector* pParent = (MOS_AgentSelector*)parent();
        return pParent->IsActive( bMove );
    }
    else
    if( nType_ == eListViewEditor )
    {
        MOS_AgentEditor* pParent = static_cast< MOS_AgentEditor* >( parent()->parent()->parent() ); // Tab -> TabWidget -> MOS_AgentEditor
        return pParent->IsActive( bMove );
    }
    else
    if( nType_ == eListViewMission )
    {
        MOS_AgentMission* pParent = (MOS_AgentMission*)parent();
        return pParent->IsActive( bMove );
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_AgentListView::Draw()
{
    if( nType_ == eListViewMission && pAgent_ )
    {
        GFX_Color color( 0., 0.,255 );

        for( CIT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end() ; ++itAgent )
        {
            MOS_AgentListViewItem_ABC* pCurAgent = (MOS_AgentListViewItem_ABC*)*itAgent;
            GFX_Tools::CreateGLLine( pAgent_->GetPos(), pCurAgent->GetAgent().GetPos(), 2.0, color );
        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::MOS_AgentMission
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_AgentListView::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
{
    if( IsActive( true ) && bFollow_ )
    {
        if( pSelectedAgent_ )
        {
            if( nType_ == eListViewSelector )
            {
                MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)pSelectedAgent_;
                if( &pAgent->GetAgent() == &agent )
                {
                    LocOnAgent( pSelectedAgent_, false );
                }
            }
            else
            {
                MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)pSelectedAgent_;
                if( &pAgent->GetAgent() == &agent )
                {
                    LocOnAgent( pSelectedAgent_, false );
                }
            }

        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::NotifyDeleteAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentListView::NotifyDeleteAgent( MOS_Agent& agent )
{
    pSelectedAgent_ = 0;

    if( nType_ == eListViewEditor )   
    {
        if( agent.GetModelPion() )
        {
            const MOS_AgentModel::T_Mission_Vector& vAvailableMissions = agent.GetModelPion()->GetAvailableMissions();
            for( MOS_AgentModel::CIT_Mission_Vector it = vAvailableMissions.begin(); it != vAvailableMissions.end(); ++it )
            {
                E_MissionID nMission = (*it);

                IT_MissionDialogPtrMap found = missionDialogPtrMap_.find( nMission );
                if( found == missionDialogPtrMap_.end() )
                    continue;

                if( (found->second)->GetAgent() == & agent )
                {
                    (found->second)->Cancel();
                    return;
                }
            }
        }
        if( agent.GetModelAutomate() )
        {
            const MOS_AgentModel::T_Mission_Vector& vAvailableMissions = agent.GetModelAutomate()->GetAvailableMissions();
            for( MOS_AgentModel::CIT_Mission_Vector it = vAvailableMissions.begin(); it != vAvailableMissions.end(); ++it )
            {
                E_MissionID nMission = (*it);

                IT_MissionDialogPtrMap found = missionDialogPtrMap_.find( nMission );
                if( found == missionDialogPtrMap_.end() )
                    continue;

                if( (found->second)->GetAgent() == & agent )
                {
                    (found->second)->Cancel();
                    return;
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::CreateAgentFromSelector
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_AgentListView::CreateAgentFromSelector( MOS_AgentListView* pSelector )
{
    assert( nType_ == eListViewMission );

    RIT_AgentPtrVector itAgentItem = agentVector_.rbegin();
    while( itAgentItem != agentVector_.rend() )
    {
        MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)*itAgentItem;
        pAgent->GetAgent().UnregisterListViewItem( this, pAgent );
        DeleteAgent( *pAgent );
        itAgentItem = agentVector_.rbegin();
    }

    MOS_AgentManager::T_AgentPtrVector agentVector;
    pSelector->GetTaggedAgents( agentVector );

    for( MOS_AgentManager::IT_AgentPtrVector itAgent = agentVector.begin() ; itAgent != agentVector.end() ; ++itAgent )
    {

        MOS_Agent& agent = **itAgent;

        MOS_AgentListViewItem_ABC* pAgent = 0;
        if( agentVector_.size() )
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this, **(agentVector_.rbegin()) );
        else
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this );
        assert( pAgent );
        RegisterAgent( *pAgent );
        pAgent->SetName( agent.GetName() );

        agent.RegisterListViewItem( this, pAgent );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetTaggedAgents
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_AgentListView::GetTaggedAgents( MOS_AgentManager::T_AgentPtrVector& agentVector )
{
    assert( nType_ == eListViewSelector );
    for( IT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end() ; ++itAgent )
    {
        MOS_AgentCheckListItem_ABC* pAgent = (MOS_AgentCheckListItem_ABC*)*itAgent;
        if( pAgent->isOn() )
            agentVector.push_back( &pAgent->GetAgent() );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::GetInitAgent
// Created: FBD 03-02-07
//-----------------------------------------------------------------------------
MOS_Agent* MOS_AgentListView::GetInitAgent( const MOS_Agent& agent )
{
    MOS_Agent* pInitAgent = 0;

    MT_Float rDist = 9999999999.0;
    MOS_AgentManager::CT_AgentMap& listAgent = MOS_App::GetApp().GetAgentManager().GetAgentList();
    {
        for( MOS_AgentManager::CIT_AgentMap itAgent = listAgent.begin() ; itAgent != listAgent.end() ; ++itAgent )
        {
            if( itAgent->second != &agent)
            {
                MT_Float rCurdist = itAgent->second->GetPos().Distance( agent.GetPos() );
                if( rCurdist < rDist )
                {
                    rDist  = rCurdist;
                    pInitAgent = itAgent->second;
                }
            }
        }
    }
    return pInitAgent;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SetAgent
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_AgentListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
    if( nType_ == eListViewMission && pMissionTest_ )
    {
        MOS_Agent* pInitAgent = GetInitAgent( *pAgent );
        if( pInitAgent )
        {
            MOS_AgentListViewItem_ABC* pAgent = 0;
            if( agentVector_.size() )
                pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, *pInitAgent, *this, **(agentVector_.rbegin()) );
            else
                pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, *pInitAgent, *this );
            assert( pAgent );
            RegisterAgent( *pAgent );
            pAgent->SetName( pInitAgent->GetName() );

            pInitAgent->RegisterListViewItem( this, pAgent );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::WriteMssg
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_AgentListView::WriteMsg( ASN1T_ListAgent& asnListAgent, std::string& sParam )
{
    assert( nType_ == eListViewMission );

    asnListAgent.n    = agentVector_.size();
    asnListAgent.elem = new ASN1T_OID[ agentVector_.size() ];
    sParam += MT_FormatString( "%d Agents " , agentVector_.size() );

    int i = 0;
    for( IT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end() ; ++itAgent )
    {
        MOS_AgentListViewItem_ABC* pAgentItem = (MOS_AgentListViewItem_ABC*)(*itAgent);
        pAgentItem->WriteMsg( asnListAgent.elem[i], sParam );
        ++i;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::WriteMsg
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
void MOS_AgentListView::WriteMsg( ASN1T_Agent& asnAgent, std::string& sParam )              
{
    assert( nType_ == eListViewMission );
    assert( agentVector_.size() == 1 );
    sParam += std::string( "1 agent " );

    MOS_AgentListViewItem_ABC* pAgentItem = (MOS_AgentListViewItem_ABC*)*agentVector_.begin();
    pAgentItem->WriteMsg( asnAgent, sParam );
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentListView::FillRandomParameters()
{
    /*assert( pAgent_ );
    assert( nType_ == eListViewMission );

    int nTeam = pAgent_->GetTeam();
    RIT_AgentPtrVector itAgentItem = agentVector_.rbegin();
    while( itAgentItem != agentVector_.rend() )
    {
        MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)*itAgentItem;
        pAgent->GetAgent().UnregisterListViewItem( this, pAgent );
        DeleteAgent( *pAgent );
        itAgentItem = agentVector_.rbegin();
    }

    MOS_AgentListView::T_AgentPtrVector& agentVectorEditor = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GetListAgent();
    if( agentVectorEditor.size() < 1 )
        return;


    MOS_AgentManager::T_AgentPtrVector agentVector;
    uint nWantedAgent = 1; //rand() % 2; // agentVectorEditor.size() + 1;
    uint nTry = 255;
    while ( agentVector.size() < nWantedAgent && nTry )
    {
        --nTry;
        for( MOS_AgentListView::IT_AgentPtrVector itAgent = agentVectorEditor.begin() ; itAgent != agentVectorEditor.end() ; ++itAgent )
        {
            MOS_AgentListViewItem_ABC* pAgentItem = (MOS_AgentListViewItem_ABC*)*itAgent;
            assert( pAgentItem );
            MOS_Agent* pAgent = &pAgentItem->GetAgent();
            MOS_AgentManager::IT_AgentPtrVector itFound = std::find( agentVector.begin() , agentVector.end(), pAgent );
            if( itFound == agentVector.end() )
            {
                if( pAgent->GetTeam() == nTeam )
                {
                    if( pAgent->GetAgentID() != pAgent_->GetAgentID() )
                    {
                        uint nRand = rand() % 2;
                        if( nRand == 1 )
                        {
                            agentVector.push_back( pAgent );
                            nTry = 255;
                            break;
                        }
                    }
                }
            }
        }
    }

    for( MOS_AgentManager::IT_AgentPtrVector itAgent = agentVector.begin() ; itAgent != agentVector.end() ; ++itAgent )
    {

        MOS_Agent& agent = **itAgent;

        MOS_AgentListViewItem_ABC* pAgent = 0;
        if( agentVector_.size() )
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this, **(agentVector_.rbegin()) );
        else
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this );
        assert( pAgent );
        RegisterAgent( *pAgent );
        pAgent->SetName( agent.GetName() );

        agent.RegisterListViewItem( this, pAgent );
    }
*/

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentListView::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    assert( pAgent_ );
    assert( nType_ == eListViewMission );

    MOS_AgentListView::T_AgentPtrVector& agentVectorEditor = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GetListAgent();
    if( agentVectorEditor.size() < 1 )
        return;


    uint nNbAgent = agentVector_.size();
    archive.WriteField( "sName", std::string( "MOS_AgentListView|nNbAgent" ) );
    archive.WriteField( "nNbAgent", nNbAgent );

    for( MOS_AgentListView::IT_AgentPtrVector itAgent = agentVector_.begin() ; itAgent != agentVector_.end() ; ++itAgent )
    {
        MOS_AgentListViewItem_ABC* pAgentItem = (MOS_AgentListViewItem_ABC*)*itAgent;

        uint nAgent = 0;
        for( MOS_AgentListView::IT_AgentPtrVector itAgentEditor = agentVectorEditor.begin() ; itAgentEditor != agentVectorEditor.end() ; ++itAgentEditor  )
        {
            MOS_AgentListViewItem_ABC* pAgentItemDest = (MOS_AgentListViewItem_ABC*)*itAgentEditor;

            if( &pAgentItem->GetAgent() == &pAgentItemDest->GetAgent() )
            {
                archive.WriteField( "sName", std::string( "MOS_AgentListView|nAgent" ) );
                archive.WriteField( "nAgent", nAgent );
                break;
            }
            ++nAgent;
        }
    }


}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentListView::ReloadParameters( MT_InputArchive_ABC& archive )
{
    assert( nType_ == eListViewMission );

    RIT_AgentPtrVector itAgentItem = agentVector_.rbegin();
    while( itAgentItem != agentVector_.rend() )
    {
        MOS_AgentListViewItem_ABC* pAgent = (MOS_AgentListViewItem_ABC*)*itAgentItem;
        pAgent->GetAgent().UnregisterListViewItem( this, pAgent );
        DeleteAgent( *pAgent );
        itAgentItem = agentVector_.rbegin();
    }

    MOS_AgentListView::T_AgentPtrVector& agentVectorEditor = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().GetListAgent();
    if( agentVectorEditor.size() < 1 )
        return;


    uint nNbAgent = 0;
    std::string sName( "MOS_AgentListView|nNbAgent" );
    archive.ReadField( "sName", sName );
    archive.ReadField( "nNbAgent", nNbAgent );

    MOS_AgentManager::T_AgentPtrVector agentVector;
    for( uint j = 0 ; j < nNbAgent ; ++j )
    {
        uint nAgent = 0;
        std::string sName( "MOS_AgentListView|nAgent" );
        archive.ReadField( "sName", sName );
        archive.ReadField( "nAgent", nAgent );
        agentVector.push_back( &((MOS_AgentListViewItem_ABC*)agentVectorEditor[nAgent])->GetAgent() );
    }


    for( MOS_AgentManager::IT_AgentPtrVector itAgent = agentVector.begin() ; itAgent != agentVector.end() ; ++itAgent )
    {

        MOS_Agent& agent = **itAgent;

        MOS_AgentListViewItem_ABC* pAgent = 0;
        if( agentVector_.size() )
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this, **(agentVector_.rbegin()) );
        else
            pAgent = new MOS_AgentListViewItem_ABC( eItemTypeAgent, agent, *this );
        assert( pAgent );
        RegisterAgent( *pAgent );
        pAgent->SetName( agent.GetName() );

        agent.RegisterListViewItem( this, pAgent );
    }


}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::MOS_AgentEditor
// Created: FBD 03-02-06
//-----------------------------------------------------------------------------
void MOS_AgentListView::NotifyAgentMissionAck( ASN1T_EnumOrderErrorCode nErrorCode )
{
    nStateTest_ = 1;
    if( nErrorCode == EnumOrderErrorCode::no_error )
        printf( "Mission OK\n" );
    else
        printf( "Mission Error\n" );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView constructor
// Created: FBD 03-02-07
//-----------------------------------------------------------------------------
void MOS_AgentListView::NotifyDisconnected()
{
    nStateTest_ = 2;
}

//=============================================================================
// DRAG AND DROP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::AcceptAgentChangeAutomate
// Created: NLD 2003-05-06
//-----------------------------------------------------------------------------
void MOS_AgentListView::AcceptAgentChangeAutomate( uint nID )
{
    IT_AgentChangeAutomateMap itAgent = agentChangeAutomateMap_.find( nID );
    if( itAgent == agentChangeAutomateMap_.end() )
        return;

    agentChangeAutomateIDManager_.ReleaseIdentifier( nID );

    T_AgentChangeAutomatePair& agentPair = itAgent->second;

    MOS_PionListViewItem*       pPionItem        = agentPair.first;
    MOS_AutomateListViewItem*   pNewAutomateItem = agentPair.second;

    MOS_Agent& pion     = pPionItem       ->GetAgent();
    MOS_Agent& automate = pNewAutomateItem->GetAgent();

    pion.OnAutomateChanged( automate );
    agentChangeAutomateMap_.erase( itAgent );
    pSelectedAgent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::AcceptAutomateChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentListView::AcceptAutomateChangeKnowledgeGroup( uint nID )
{
    IT_AutomateChangeKnowledgeGroupMap itAutomate = automateChangeKnowledgeGroupMap_.find( nID );
    if( itAutomate == automateChangeKnowledgeGroupMap_.end() )
        return;

    agentChangeAutomateIDManager_.ReleaseIdentifier( nID );

    T_AutomateChangeKnowledgeGroupPair& automatePair = itAutomate->second;

    MOS_AutomateListViewItem*       pAutomateItem = automatePair.first;
    MOS_KnowledgeGroupListViewItem* pNewKGItem   = automatePair.second;

    MOS_Agent& automate = pAutomateItem->GetAgent();
    MOS_Gtia& gtia      = pNewKGItem   ->GetGtia();

    automate.OnKnowledgeGroupChanged( gtia );
    automateChangeKnowledgeGroupMap_.erase( itAutomate );

    //$$$ Ooooooouh, c'est pas bô !
    pAutomateItem->parent()->takeItem  ( pAutomateItem );
    pNewKGItem             ->insertItem( pAutomateItem );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::CancelAgentChangeAutomate
// Created: NLD 2003-05-06
//-----------------------------------------------------------------------------
void MOS_AgentListView::CancelAgentChangeAutomate( uint nID )
{
    if( agentChangeAutomateMap_.erase( nID ) == 1 )
        agentChangeAutomateIDManager_.ReleaseIdentifier( nID );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::CancelAutomateChangeKnowledgeGroup
// Created: NLD 2003-05-06
//-----------------------------------------------------------------------------
void MOS_AgentListView::CancelAutomateChangeKnowledgeGroup( uint nID )
{
    if( automateChangeKnowledgeGroupMap_.erase( nID ) == 1 )
        agentChangeAutomateIDManager_.ReleaseIdentifier( nID );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::dragEnterEvent
// Created: NLD 2003-05-05
//-----------------------------------------------------------------------------
void MOS_AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    //if( e->provides( "text/dragdemotag" ) )
    pEvent->accept();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::dropEvent
// Created: NLD 2003-05-05
//-----------------------------------------------------------------------------
void MOS_AgentListView::dropEvent( QDropEvent* pEvent )
{
    if( pEvent->provides( "text/pion" ) )
    {
        QByteArray tmp = pEvent->encodedData( "text/pion" );

        MOS_ListViewItem_ABC* pItemToDrop;
        memcpy( &pItemToDrop, tmp.data(), sizeof( pItemToDrop ) );

        MOS_ListViewItem_ABC* pItemWhereToDrop = (MOS_ListViewItem_ABC*)itemAt( viewport()->mapFromParent( pEvent->pos() ) );

        if( !pItemToDrop || pItemToDrop->GetItemType() != eItemTypePion
            ||  !pItemWhereToDrop || pItemWhereToDrop->GetItemType() != eItemTypeAutomate )
        {
            pEvent->ignore();
            return;
        }

        uint nID = agentChangeAutomateIDManager_.GetFreeIdentifier();
        T_AgentChangeAutomatePair agentPair( (MOS_PionListViewItem*)pItemToDrop, (MOS_AutomateListViewItem*)pItemWhereToDrop );
        agentChangeAutomateMap_.insert( std::make_pair( nID, agentPair ) );

        // Envoi réseau
        MOS_ASN_MsgChangeAutomate asnMsg;
        asnMsg.GetAsnMsg().oid_pion      = ((MOS_PionListViewItem*)pItemToDrop)->GetAgent().GetAgentID();
        asnMsg.GetAsnMsg().oid_automate  = ((MOS_AutomateListViewItem*)pItemWhereToDrop)->GetAgent().GetAgentID();
        asnMsg.Send( nID );

        pEvent->accept();
    }

    else if( pEvent->provides( "text/automate" ) )
    {
        QByteArray tmp = pEvent->encodedData( "text/automate" );

        MOS_ListViewItem_ABC* pItemToDrop;
        memcpy( &pItemToDrop, tmp.data(), sizeof( pItemToDrop ) );

        MOS_ListViewItem_ABC* pItemWhereToDrop = (MOS_ListViewItem_ABC*)itemAt( viewport()->mapFromParent( pEvent->pos() ) );

        if( !pItemToDrop || pItemToDrop->GetItemType() != eItemTypeAutomate
            || !pItemWhereToDrop || pItemWhereToDrop->GetItemType() != eItemTypeKnowledgeGroup )
        {
            pEvent->ignore();
            return;
        }

        uint nID = agentChangeAutomateIDManager_.GetFreeIdentifier();
        T_AutomateChangeKnowledgeGroupPair pair( (MOS_AutomateListViewItem*)pItemToDrop, (MOS_KnowledgeGroupListViewItem*)pItemWhereToDrop );
        automateChangeKnowledgeGroupMap_.insert( std::make_pair( nID, pair ) );

        // Envoi réseau
        const MOS_Gtia& gtia = ((MOS_KnowledgeGroupListViewItem*)pItemWhereToDrop)->GetGtia();

        MOS_ASN_MsgChangeGroupeConnaissance asnMsg;
        asnMsg.GetAsnMsg().oid_automate            = ((MOS_AutomateListViewItem*)pItemToDrop)->GetAgent().GetAgentID();
        asnMsg.GetAsnMsg().oid_camp                = gtia.GetTeam().GetID();
        asnMsg.GetAsnMsg().oid_groupe_connaissance = gtia.GetID();
        asnMsg.Send( nID );

        pEvent->accept();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::dragObject
// Created: NLD 2003-04-30
// Last modified: JVT 03-09-30
//-----------------------------------------------------------------------------
QDragObject* MOS_AgentListView::dragObject()
{
    MOS_ListViewItem_ABC* pItem = (MOS_ListViewItem_ABC*)selectedItem();
    if( !pItem )
        return 0;

    if( pItem->GetItemType() == eItemTypePion )
    {
        QByteArray* bytes = new QByteArray(); // $$$$ JVT - 01/01/2000 : Pourquoi ????
        bytes->setRawData( (const char*)&pItem, sizeof( MOS_ListViewItem_ABC* ) );
        QStoredDrag* data = new QStoredDrag( "text/pion", this );
        data->setEncodedData( *bytes );
        return data;
    }
        
    if( pItem->GetItemType() == eItemTypeAutomate )
    {
        QByteArray* bytes = new QByteArray(); // $$$$ NLD - 01/12/2004 : Parce que !!!!
        bytes->setRawData( (const char*)&pItem, sizeof( MOS_ListViewItem_ABC* ) );
        QStoredDrag* data = new QStoredDrag( "text/automate", this );
        data->setEncodedData( *bytes );
        return data;
    }
    return 0;    
}
