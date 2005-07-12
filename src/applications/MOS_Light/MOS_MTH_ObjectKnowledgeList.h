//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ObjectKnowledgeList.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_MTH_ObjectKnowledgeList.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_ObjectKnowledgeList_h_
#define __MOS_MTH_ObjectKnowledgeList_h_


#include <QListView.h>
#include <QObjectDefs.h>

#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

class QWidget;
class QVBoxLayout;

//*****************************************************************************
// Created: AGN 03-04-23
//*****************************************************************************
class MOS_MTH_ObjectKnowledgeList
: public QListView
, public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MTH_ObjectKnowledgeList )

public:
    MOS_MTH_ObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnKnowledgeObjectList, QWidget* pParent, const std::string& sName );
    MOS_MTH_ObjectKnowledgeList( ASN1T_KnowledgeObject& asnKnowledgeObject, QWidget* pParent, const std::string& sName );
    virtual ~MOS_MTH_ObjectKnowledgeList();

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
    ASN1T_ListKnowledgeObject* pASNKnowledgeObjectList_;
    ASN1T_KnowledgeObject*     pASNKnowledgeObject_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_ObjectKnowledgeList.inl"
#endif


#endif // __MOS_MTH_ObjectKnowledgeList_h_