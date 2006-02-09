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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentKnowledgeList.h $
// $Author: Ape $
// $Modtime: 7/01/05 10:41 $
// $Revision: 4 $
// $Workfile: ParamAgentKnowledgeList.h $
//
// *****************************************************************************

#ifndef __ParamAgentKnowledgeList_h_
#define __ParamAgentKnowledgeList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class Gtia;
class AgentKnowledge;
class Agent_ABC;


// =============================================================================
/** @class  ParamAgentKnowledgeList
    @brief  ParamAgentKnowledgeList
    @par    Using example
    @code
    ParamAgentKnowledgeList;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class ParamAgentKnowledgeList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamAgentKnowledgeList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamAgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, bool bOptional );
    ~ParamAgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuKnowledge();
    void OnAgentKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListKnowledgeAgent& asnListKnowledge_;
    ASN1T_OID* pAsnOIDList_;
    Agent_ABC& agent_;
    int nMinItems_;
    int nMaxItems_;

    AgentKnowledge* pPopupKnowledge_;
    //@}
};

#endif // __ParamAgentKnowledgeList_h_
