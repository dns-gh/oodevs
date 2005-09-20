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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentKnowledge.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: MOS_ParamAgentKnowledge.h $
//
// *****************************************************************************

#ifndef __MOS_ParamAgentKnowledge_h_
#define __MOS_ParamAgentKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_AgentKnowledge;
class MOS_Gtia;
class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamAgentKnowledge
    @brief  MOS_ParamAgentKnowledge
    @par    Using example
    @code
    MOS_ParamAgentKnowledge;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class MOS_ParamAgentKnowledge : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamAgentKnowledge );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamAgentKnowledge( ASN1T_KnowledgeAgent& asnKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent );
    ~MOS_ParamAgentKnowledge();
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
    std::string strMenuText_;
    ASN1T_KnowledgeAgent& asnKnowledge_;
    MOS_Agent& agent_;
    MOS_AgentKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    MOS_AgentKnowledge* pPopupKnowledge_;
};

#   include "MOS_ParamAgentKnowledge.inl"

#endif // __MOS_ParamAgentKnowledge_h_
