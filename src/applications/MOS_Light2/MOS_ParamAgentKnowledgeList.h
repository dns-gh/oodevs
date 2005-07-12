// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentKnowledgeList.h $
// $Author: Ape $
// $Modtime: 7/01/05 10:41 $
// $Revision: 4 $
// $Workfile: MOS_ParamAgentKnowledgeList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamAgentKnowledgeList_h_
#define __MOS_ParamAgentKnowledgeList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_Gtia;
class MOS_AgentKnowledge;
class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamAgentKnowledgeList
    @brief  MOS_ParamAgentKnowledgeList
    @par    Using example
    @code
    MOS_ParamAgentKnowledgeList;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class MOS_ParamAgentKnowledgeList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamAgentKnowledgeList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent );
    ~MOS_ParamAgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuKnowledge();
    void OnAgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListKnowledgeAgent& asnListKnowledge_;
    ASN1T_OID* pAsnOIDList_;
    MOS_Agent& agent_;
    int nMinItems_;
    int nMaxItems_;

    MOS_AgentKnowledge* pPopupKnowledge_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamAgentKnowledgeList.inl"
#endif

#endif // __MOS_ParamAgentKnowledgeList_h_
