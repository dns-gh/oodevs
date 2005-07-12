//*****************************************************************************
//
// $Created: FBD 03-01-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MissionAction.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_MissionAction.h $
//
//*****************************************************************************

#ifndef __MOS_MissionAction_h_
#define __MOS_MissionAction_h_

#include "MOS_Types.h"
#include "MOS_Order_Def.h"

#include <QAction.h>

class MOS_Mission_ABC;
class MOS_AgentListView;

//*****************************************************************************
// Created: FBD 03-01-08
//*****************************************************************************
class MOS_MissionAction : public QAction
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MissionAction );


public:
    explicit MOS_MissionAction( MOS_AgentListView& listView, MOS_Mission_ABC& mission );
    virtual ~MOS_MissionAction();


    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    E_MissionID GetTypeMission() const;
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotSelectMission();
    //@}

private:
    MOS_Mission_ABC&    mission_;
    MOS_AgentListView&  listView_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MissionAction.inl"
#endif


#endif // __MOS_MissionAction_h_