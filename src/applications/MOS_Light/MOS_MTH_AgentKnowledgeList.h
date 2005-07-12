//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_AgentKnowledgeList.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_MTH_AgentKnowledgeList.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_AgentKnowledgeList_h_
#define __MOS_MTH_AgentKnowledgeList_h_


#include <QListView.h>
#include <QObjectDefs.h>

#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

class QWidget;
class QVBoxLayout;

//*****************************************************************************
// Created: AGN 03-04-23
//*****************************************************************************
class MOS_MTH_AgentKnowledgeList
: public QListView
, public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MTH_AgentKnowledgeList )

public:
    MOS_MTH_AgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListAgent, QWidget* pParent, const std::string& sName );
    MOS_MTH_AgentKnowledgeList( ASN1T_KnowledgeAgent& asnAgent, QWidget* pParent, const std::string& sName );
    virtual ~MOS_MTH_AgentKnowledgeList();

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetAgent( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteMsg( std::string& sParam );
    virtual void FillRandomParameters();
    virtual void ReloadParameters( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive );
    
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate();
    //@}
    
private slots:
    void OnClick( QListViewItem* pItem );

private:
    MOS_Agent* pAgent_;
    ASN1T_ListKnowledgeAgent* pASNAgentList_;
    ASN1T_KnowledgeAgent*     pASNAgent_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_AgentKnowledgeList.inl"
#endif


#endif // __MOS_MTH_AgentKnowledgeList_h_