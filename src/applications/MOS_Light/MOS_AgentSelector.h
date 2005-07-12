//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentSelector.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_AgentSelector.h $
//
//*****************************************************************************

#ifndef __MOS_AgentSelector_h_
#define __MOS_AgentSelector_h_

#include "MOS_Types.h"

#include <QFrame.h>

class MOS_AgentListView;
class MOS_AgentMission;
class QWidget;
class MOS_Agent;


//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_AgentSelector : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_AgentSelector );

public:
    explicit MOS_AgentSelector( QWidget* pParent = 0 );
    virtual ~MOS_AgentSelector();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool IsActive( bool bMove );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_AgentListView&       GetAgentListView () const;
    void                    SetAgentMission  ( MOS_AgentListView& AgentMission );
    void                    SetAgent        ( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Accept();
    void Cancel();
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotAccept();
    void SlotReject();
    //@}


private:
    MOS_AgentListView*   pAgentListView_;
    MOS_AgentListView*   pMissionListView_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentSelector.inl"
#endif


#endif // __MOS_AgentSelector_h_