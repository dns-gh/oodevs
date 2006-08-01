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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentKnowledge.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: ParamAgentKnowledge.h $
//
// *****************************************************************************

#ifndef __ParamAgentKnowledge_h_
#define __ParamAgentKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class AgentKnowledge;
class Gtia;
class Agent_ABC;


// =============================================================================
// Created: APE 2004-05-10
// =============================================================================
class ParamAgentKnowledge : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamAgentKnowledge );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamAgentKnowledge( ASN1T_KnowledgeAgent& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamAgentKnowledge();
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
    std::string strMenuText_;
    ASN1T_KnowledgeAgent& asnKnowledge_;
    Agent_ABC& agent_;
    AgentKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    AgentKnowledge* pPopupKnowledge_;
};

#endif // __ParamAgentKnowledge_h_
