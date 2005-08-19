//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_ABC.cpp $
// $Author: Nld $
// $Modtime: 14/01/05 16:01 $
// $Revision: 95 $
// $Workfile: MOS_Mission_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_Mission_ABC.h"
#include "moc_MOS_Mission_ABC.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_Mission_ABC.inl"
#endif

#include "MOS_LimaMission.h"
#include "MOS_LimitMission.h"
#include "MOS_MTH_Itineraire1Pos.h"
#include "MOS_MTH_ItinerairePos.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentListView.h"
#include "MOS_LimaListView.h"
#include "MOS_LimitListView.h"
#include "MOS_AgentMission.h"
#include "MOS_Value.h"
#include "MOS_ValueList.h"
#include "MOS_MTH_SegmentPos.h"
#include "MOS_MTH_DirectionPos.h"
#include "MOS_MissionTool_ABC.h"
#include "MOS_MTH_GDH.h"
#include "MOS_ASN_Messages.h"
#include "MOS_MTH_AgentKnowledgeList.h"
#include "MOS_MTH_ObjectKnowledgeList.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_ListLocalisation_ListView.h"
#include "MOS_MTH_PointList_ListView.h"
#include "MOS_MTH_Obstacle_ListView.h"
#include "MOS_MTH_NatureAtlas.h"
#include "MOS_History.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_TestManager.h"
#include "MOS_MissionAction.h"
#include "MOS_MTH_Obstacle.h"
#include "MOS_MTH_ListPoint.h"
#include "MOS_MTH_MaintenancePriorities.h"
#include "MOS_MTH_MedicalPriorities.h"

#include <QButtonGroup.h>
#include <QWidgetStack.h>

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC constructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
MOS_Mission_ABC::MOS_Mission_ABC( E_MissionID nTypeMission, QWidget* pParent )
    : QWidget           ( pParent )
    , MOS_GLMouse_ABC   ()
    , nTypeMission_     ( nTypeMission )
    , pOptionLayout_    ( 0 )
    , pOptionWidget_    ( 0 )
    , pTabWidget_       ( 0 )
    , pLabel_           ( 0 )
    , pLabelLayout_     ( 0 )
    , pMainLayout_      ( 0 )
    , pAgent_           ( 0 )
    , nNbLinkGLWidget_  ( 0 )
    , pAction_( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC destructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
MOS_Mission_ABC::~MOS_Mission_ABC()
{
    delete pAction_;
}


//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::Initialize
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::Initialize()
{
    nNbLinkGLWidget_ = MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().GetNbLink();
    MOS_GLMouse_ABC::Initialize();

    /////////////////////////////////////////
    // Main layout
    pMainLayout_ = new QVBoxLayout( this );
    pMainLayout_->setMargin( 2 );
    pMainLayout_->setSpacing( 10 );

    // Nom de la mission
    pLabelLayout_ = new QHBoxLayout( pMainLayout_ );
    std::string sName( "Mission " );
    sName += GetName();
    pLabel_ = new QLabel( sName.c_str(), this );
    pLabel_->setAlignment(AlignHCenter );
    pLabel_->setMargin( 5 );
    pLabel_->setLineWidth( 2 );
    pLabel_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout_->addWidget( pLabel_ );

    // Tab
    pTabWidget_ = new QTabWidget( this );
    pMainLayout_->addWidget( pTabWidget_ );

    //--------------------
    // Parameters specifics
    pOptionWidget_ = new QTabWidget( this );
    pTabWidget_->addTab( pOptionWidget_, "Mission" );
    pOptionLayout_ = new QVBoxLayout( pOptionWidget_ );

    //--------------------
    // OK / Cancel
    pButtonGroup_ = new QButtonGroup( 1, Vertical, this );
    pOKButton_ = new QPushButton( tr("OK"), pButtonGroup_ );
    pCancelButton_ = new QPushButton( tr("Cancel"), pButtonGroup_ );
    pOKButton_->setDefault( TRUE );
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout_ );
    pButtonLayout->addWidget( pButtonGroup_ );
    connect( pCancelButton_, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton_    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );

}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::Terminate
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::Terminate()
{
    if( pCancelButton_ )
    {
        delete pCancelButton_;
        pCancelButton_ = 0;
    }

    if( pOKButton_ )
    {
        delete pOKButton_;
        pOKButton_ = 0;
    }

    if( pButtonGroup_ )
    {
        delete pButtonGroup_;
        pButtonGroup_ = 0;
    }

    for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
    {
        (*itMissionTool)->Terminate();
        delete *itMissionTool;
    }
    missionToolVector_.clear();


    if( pOptionLayout_ )
    {
        delete pOptionLayout_;
        pOptionLayout_ = 0;
    }

    if( pOptionWidget_ )
    {
        delete pOptionWidget_;
        pOptionWidget_ = 0;
    }

    if( pTabWidget_ )
    {
        delete pTabWidget_;
        pTabWidget_ = 0;
    }

    if( pLabel_ )
    {
        delete pLabel_;
        pLabel_ = 0;
    }

    if( pLabelLayout_ )
    {
        delete pLabelLayout_;
        pLabelLayout_ = 0;
    }

    if( pMainLayout_ )
    {
        delete pMainLayout_;
        pMainLayout_ = 0;
    }

    pAgent_ = 0;

    MOS_GLMouse_ABC::Terminate();

    assert( MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().GetNbLink() == nNbLinkGLWidget_ );
}

//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::Validate
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::Validate()
{
    if ( MOS_App::GetApp().GetTestManager().GetFlagRec() )
        MOS_App::GetApp().GetTestManager().RecordManual( MOS_App::GetApp().GetTime(), *pAgent_, *this );

    std::string sParam;

    sParam += "Pion mission : ";
    sParam += GetName();
    sParam += " | ";
        
    SendMsg( sParam );

    uint nTime = (uint)MOS_App::GetApp().GetTime();
    MOS_App::GetApp().GetMainWindow().GetHistory().AddMission( *pAgent_, GetName(), (uint)nTime, (uint)-1, sParam, nTypeMission_ );
    //  MOS_App::GetApp().GetMainWindow().GetHistory().StartPopup( sParam );
  
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
    Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::Cancel
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
    Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::SlotValidate
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::SlotValidate()
{
    Validate();
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::SlotCancel
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::SlotCancel()
{
    Cancel();
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::SetAgent
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    if( pAgent )
    {
        MOS_App::GetApp().GetMainWindow().GetLimaEditor   ().SetAgent( pAgent_ );
        MOS_App::GetApp().GetMainWindow().GetLimaSelector ().SetAgent( pAgent_ );
        MOS_App::GetApp().GetMainWindow().GetLimitEditor  ().SetAgent( pAgent_ );
        MOS_App::GetApp().GetMainWindow().GetLimitSelector().SetAgent( pAgent_ );

        for( IT_MissionToolPtrVector itMissionTool = missionToolVector_.begin() ; itMissionTool != missionToolVector_.end() ; ++itMissionTool )
            (*itMissionTool)->SetAgent( pAgent_ );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::Draw
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::Draw()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::IsActive
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
bool MOS_Mission_ABC::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if( pStack && pStack->visibleWidget() == this )
        return true;
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::IsActiveTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
bool MOS_Mission_ABC::IsActiveTab( QWidget* pWidget )
{
   if( pTabWidget_->currentPage() == pWidget )
       return true;
   return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::RMBPressEvent
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
bool MOS_Mission_ABC::RMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::LMBPressEvent
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
bool MOS_Mission_ABC::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::MouseMoveEvent
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::MouseMoveEvent( const MT_Vector2D& /*vPos*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::RMBReleaseEvent
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::LMBReleaseEvent
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::RegisterToolMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::RegisterMissionTool( MOS_MissionTool_ABC& missionTool )
{
    assert( std::find( missionToolVector_.begin(), missionToolVector_.end(), &missionTool ) == missionToolVector_.end()  );
    missionToolVector_.push_back( &missionTool );
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::UnregisterToolMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_Mission_ABC::UnregisterMissionTool( MOS_MissionTool_ABC& missionTool )
{
    IT_MissionToolPtrVector itMissionTool = std::find( missionToolVector_.begin(), missionToolVector_.end(), &missionTool );
    assert( itMissionTool != missionToolVector_.end()  );
    missionToolVector_.erase( itMissionTool );
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateLimitMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
MOS_LimitMission& MOS_Mission_ABC::CreateLimitMission( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& /*sName*/ )
{
    MOS_LimitMission* pMissionTool = new MOS_LimitMission( asnLeftLimitID, asnRightLimitID, pParentWidget );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateLimaMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
MOS_LimaMission& MOS_Mission_ABC::CreateLimaMission( ASN1T_ListOID& asnLimas, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& /*sName*/ )
{
    MOS_LimaMission* pMissionTool = new MOS_LimaMission( asnLimas, pParentWidget );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateDirection
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
MOS_MTH_DirectionPos& MOS_Mission_ABC::CreateDirection( ASN1T_Direction& asnDirection, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, bool bFollow )
{
    MOS_MTH_DirectionPos* pMissionTool = new MOS_MTH_DirectionPos( asnDirection, pParentWidget, sName, bFollow );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateSegmentPos
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
MOS_MTH_SegmentPos& MOS_Mission_ABC::CreateSegmentPos( ASN1T_Segment& asnSegment, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_SegmentPos* pMissionTool = new MOS_MTH_SegmentPos( asnSegment, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreatePoint
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
MOS_MTH_Itineraire1Pos& MOS_Mission_ABC::CreatePoint( ASN1T_Point& asnPoint, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_Itineraire1Pos* pMissionTool = new MOS_MTH_Itineraire1Pos( asnPoint, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}



//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreatePath
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
MOS_MTH_ItinerairePos& MOS_Mission_ABC::CreatePath( ASN1T_Itineraire& asnListPoint, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ItinerairePos* pMissionTool = new MOS_MTH_ItinerairePos( asnListPoint, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreatePointList
// Created: NLD 2004-05-14
// -----------------------------------------------------------------------------
MOS_MTH_ListPoint& MOS_Mission_ABC::CreatePointList( ASN1T_ListPoint& asnListPoint              , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ListPoint* pMissionTool = new MOS_MTH_ListPoint( asnListPoint, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
    
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateLocation
// Created: AGN 03-07-22
//-----------------------------------------------------------------------------
MOS_MTH_Localisation& MOS_Mission_ABC::CreateLocation( ASN1T_Localisation& asnLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_Localisation* pMissionTool = new MOS_MTH_Localisation( asnLocalisation, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateLocationList
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListView& MOS_Mission_ABC::CreateLocationList( ASN1T_ListLocalisation& asnListLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ListLocalisation_ListView* pMissionTool = new MOS_MTH_ListLocalisation_ListView( asnListLocalisation, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateLocationList
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListView& MOS_Mission_ABC::CreatePolygonList( ASN1T_ListPolygon& asnListLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ListLocalisation_ListView* pMissionTool = new MOS_MTH_ListLocalisation_ListView( asnListLocalisation, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateGenObjectList
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_ListView& MOS_Mission_ABC::CreateGenObjectList( ASN1T_ListMissionGenObject& asnListObstacle, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_Obstacle_ListView* pMissionTool = new MOS_MTH_Obstacle_ListView( asnListObstacle, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateGenObject
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle& MOS_Mission_ABC::CreateGenObject( ASN1T_MissionGenObject& asnListObstacle, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_Obstacle* pMissionTool = new MOS_MTH_Obstacle( asnListObstacle, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
    
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateMaintenancePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
MOS_MTH_MaintenancePriorities& MOS_Mission_ABC::CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_MaintenancePriorities* pMissionTool = new MOS_MTH_MaintenancePriorities( asn, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateMedicalPriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
MOS_MTH_MedicalPriorities& MOS_Mission_ABC::CreateMedicalPriorities( ASN1T_SantePriorites& asn, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_MedicalPriorities* pMissionTool = new MOS_MTH_MedicalPriorities( asn, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreatePointListList
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_ListView& MOS_Mission_ABC::CreatePathList( ASN1T_ListItineraire& asnListLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_PointList_ListView* pMissionTool = new MOS_MTH_PointList_ListView( asnListLocalisation, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateObjectKnowledgeList
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeList& MOS_Mission_ABC::CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListObject, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ObjectKnowledgeList* pMissionTool = new MOS_MTH_ObjectKnowledgeList( asnListObject, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateObjectKnowledge
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeList& MOS_Mission_ABC::CreateObjectKnowledge( ASN1T_KnowledgeObject& asnListObject, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_ObjectKnowledgeList* pMissionTool = new MOS_MTH_ObjectKnowledgeList( asnListObject, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}



//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateAgentKnowledgeList
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeList& MOS_Mission_ABC::CreateAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListAgent, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_AgentKnowledgeList* pMissionTool = new MOS_MTH_AgentKnowledgeList( asnListAgent, pParentWidget, sName );;
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateAgentKnowledge
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeList& MOS_Mission_ABC::CreateAgentKnowledge( ASN1T_KnowledgeAgent& asnListAgent, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_AgentKnowledgeList* pMissionTool = new MOS_MTH_AgentKnowledgeList( asnListAgent, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateAgentList
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
MOS_AgentMission& MOS_Mission_ABC::CreateAgentList( ASN1T_ListAgent& asnListAgent, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_AgentMission* pMissionTool = new MOS_AgentMission( asnListAgent, pParentWidget, sName, -1 );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateAutomateList
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
MOS_AgentMission& MOS_Mission_ABC::CreateAutomateList( ASN1T_ListAgent& asnListAgent, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_AgentMission* pMissionTool = new MOS_AgentMission( asnListAgent, pParentWidget, sName, -1 );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateNatureAtlas
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_NatureAtlas& MOS_Mission_ABC::CreateNatureAtlas( ASN1T_NatureAtlas& asnNatureAtlas, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_MTH_NatureAtlas* pMissionTool = new MOS_MTH_NatureAtlas( asnNatureAtlas, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateGDH
// Created: NLD 2003-03-26
//-----------------------------------------------------------------------------
MOS_MTH_GDH& MOS_Mission_ABC::CreateGDH( ASN1T_GDH& asnGDH, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, int /*nTools*/ )
{
    MOS_MTH_GDH* pMissionTool = new MOS_MTH_GDH( asnGDH, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateAgent
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
MOS_AgentMission& MOS_Mission_ABC::CreateAgent( ASN1T_Agent& asnAgent, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName )
{
    MOS_AgentMission* pMissionTool = new MOS_AgentMission( asnAgent, pParentWidget, sName );
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateOptionalAgent
// Created: AGN 03-07-22
//-----------------------------------------------------------------------------
/*MOS_AgentMission& MOS_Mission_ABC::CreateOptionalAgent( ASN1T_Agent& asnAgent, unsigned* pOption, const std::string& sName, bool bNewTab )
{
    MOS_AgentMission* pMissionTool = 0;

    if( bNewTab )
    {
        pMissionTool = new MOS_AgentMission( asnAgent, this, "", pOption );
        RegisterMissionTool( *pMissionTool );
        pMissionTool->SetWidgetTab( pMissionTool );
        pTabWidget_->addTab( pMissionTool, sName.c_str() );

    }
    else
    {
        pMissionTool = new MOS_AgentMission( asnAgent, pOptionWidget_, sName, pOption );
        RegisterMissionTool( *pMissionTool );
        pMissionTool->SetWidgetTab( pOptionWidget_ );
        pOptionLayout_->addWidget( pMissionTool );
    }
    pMissionTool->SetMission( this );
    return *pMissionTool;
} */


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::CreateValueList
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
MOS_ValueList& MOS_Mission_ABC::CreateValueList( QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, int nWidth, int nHeight )
{
    MOS_ValueList* pMissionTool = new MOS_ValueList( pParentWidget, sName, nWidth, nHeight );;
    pMissionTool->SetWidgetTab( pParentWidget );
    pMissionTool->SetMission( this );
    if( pParentLayout )
        pParentLayout->addWidget( pMissionTool );
    RegisterMissionTool( *pMissionTool );
    return *pMissionTool;
}

// -----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::GetOrCreateAction
/** @param  listView 
    @param  pParent 
    @return 
*/
// Created: AGN 2003-12-23
// -----------------------------------------------------------------------------
MOS_MissionAction* MOS_Mission_ABC::GetOrCreateAction( MOS_AgentListView& listView )
{
    if( pAction_ != 0 )
        return pAction_;

    pAction_ = new MOS_MissionAction( listView, *this );
    return pAction_;
}
