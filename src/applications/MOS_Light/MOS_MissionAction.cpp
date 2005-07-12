//*****************************************************************************
//
// $Created: FBD 03-01-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MissionAction.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_MissionAction.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MissionAction.h"
#include "moc_MOS_MissionAction.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_MissionAction.inl"
#endif

#include "MOS_AgentListView.h"
#include "MOS_Mission_ABC.h"
#include "MOS_App.h"
#include "MOS_Mainwindow.h"
 
#include <QWidgetStack.h>


//-----------------------------------------------------------------------------
// Name: MOS_MissionAction constructor
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
MOS_MissionAction::MOS_MissionAction( MOS_AgentListView& listView, MOS_Mission_ABC& mission )
    :   QAction( & listView, 0, false )
    ,   mission_( mission )
    ,   listView_( listView )
{
    std::string sName( mission.GetName() );
    setMenuText( tr( sName.c_str() ) );
    setText( tr( sName.c_str() ) );
    connect( this, SIGNAL( activated() ), this, SLOT( SlotSelectMission() ) );

}
//-----------------------------------------------------------------------------
// Name: MOS_MissionAction destructor
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
MOS_MissionAction::~MOS_MissionAction()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_MissionAction::SlotSelectMission
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
void MOS_MissionAction::SlotSelectMission()
{
    MOS_Agent* pAgent = listView_.GetSelectedAgent();
    if ( pAgent == 0 )
        return;

    mission_.Initialize();
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( &mission_ );
    mission_.SetAgent( pAgent );
    

}


//-----------------------------------------------------------------------------
// Name: MOS_MissionAction::GetType
// Created: FBD 03-01-08
//-----------------------------------------------------------------------------
E_MissionID MOS_MissionAction::GetTypeMission() const
{
    return mission_.GetTypeMission();
}

