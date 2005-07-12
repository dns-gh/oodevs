//*****************************************************************************
//
// $Created: AGN 03-05-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeList_View.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_KnowledgeList_View.h $
//
//*****************************************************************************

#ifndef __MOS_KnowledgeList_View_h_
#define __MOS_KnowledgeList_View_h_

#include "MOS_Types.h"

#include <QListView.h>
#include <QObjectDefs.h>

class MOS_AgentKnowledge;
class MOS_AgentKnowledge_Editor;
class MOS_Gtia;

//*****************************************************************************
// Created: AGN 03-05-22
//*****************************************************************************
class MOS_KnowledgeList_View : public QListView
{
    Q_OBJECT

    MT_COPYNOTALLOWED( MOS_KnowledgeList_View )

public:
    MOS_KnowledgeList_View( QWidget* pParent, const std::string& sName );
    ~MOS_KnowledgeList_View();

    void AddKnowledge   ( MOS_AgentKnowledge& knowledge );
    void RemoveKnowledge( MOS_AgentKnowledge& knowledge );

    void Initialize();

    void SetGtia( MOS_Gtia* pGtia );

private slots:
    void OnClick( QListViewItem* pItem );
    void SlotDoubleClicked( QListViewItem* pItem );

private:
    MOS_AgentKnowledge_Editor* pEditor_;
    MOS_Gtia*  pGtia_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_KnowledgeList_View.inl"
#endif


#endif // __MOS_KnowledgeList_View_h_