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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObjectKnowledge.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 5 $
// $Workfile: ParamObjectKnowledge.h $
//
// *****************************************************************************

#ifndef __ParamObjectKnowledge_h_
#define __ParamObjectKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class ObjectKnowledge;
class Team;
class Agent_ABC;


// =============================================================================
/** @class  ParamObjectKnowledge
    @brief  ParamObjectKnowledge
    @par    Using example
    @code
    ParamObjectKnowledge;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class ParamObjectKnowledge : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamObjectKnowledge );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamObjectKnowledge( ASN1T_KnowledgeObject& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamObjectKnowledge();
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
    void OnObjectKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge );
    //@}

private:
    std::string strMenuText_;
    ASN1T_KnowledgeObject& asnKnowledge_;
    Agent_ABC& agent_;
    ObjectKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    ObjectKnowledge* pPopupKnowledge_;
};

#endif // __ParamObjectKnowledge_h_
