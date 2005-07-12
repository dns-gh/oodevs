//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentMission.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 16 $
// $Workfile: MOS_AgentMission.h $
//
//*****************************************************************************

#ifndef __MOS_AgentMission_h_
#define __MOS_AgentMission_h_

#include "MOS_Types.h"

#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>

class QWidget;
class MOS_Agent;
class MOS_Mission_ABC;
class MOS_AttrEditor;
class MOS_AgentListView;


//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_AgentMission : public QWidget
                       , public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_AgentMission );

public:
    explicit MOS_AgentMission( ASN1T_ListAgent& asnListAgent, QWidget* pParent , const std::string& sName, int nNbMaxAgent = -1 );
    explicit MOS_AgentMission( ASN1T_Agent& asnAgent, QWidget* pParent , const std::string& sName, unsigned* pAgentPresent = 0 );
    virtual ~MOS_AgentMission();

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
    void SetMission( MOS_Mission_ABC* pMission );
    void SetAgent( MOS_Agent* pAgent );
    void SetWidgetTab( QWidget* pWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_AgentListView& GetAgentListView() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    void NotifyDeleteAgent( MOS_Agent& agent );
    void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

    //@}

private:
    std::string             sName_;
    ASN1T_ListAgent*        pASNListAgent_;
    ASN1T_Agent*            pASNAgent_;
    MOS_AgentListView*      pAgentListView_;
    MOS_Mission_ABC*        pMission_;
    QWidget*                pWidgetTab_;
    uint                    nNbMaxAgent_;
    unsigned*               pAgentPresent_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentMission.inl"
#endif


#endif // __MOS_AgentMission_h_