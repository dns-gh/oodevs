// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObjectKnowledgeList.h $
// $Author: Ape $
// $Modtime: 5/08/04 17:22 $
// $Revision: 4 $
// $Workfile: ParamObjectKnowledgeList.h $
//
// *****************************************************************************

#ifndef __ParamObjectKnowledgeList_h_
#define __ParamObjectKnowledgeList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class Team;
class ObjectKnowledge;
class Agent_ABC;
class Agent;


// =============================================================================
/** @class  ParamObjectKnowledgeList
    @brief  ParamObjectKnowledgeList
    @par    Using example
    @code
    ParamObjectKnowledgeList;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class ParamObjectKnowledgeList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamObjectKnowledgeList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, bool bOptional );
    ~ParamObjectKnowledgeList();
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
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListKnowledgeObject& asnListKnowledge_;
    Agent_ABC& agent_;
    ASN1T_OID* pAsnOIDList_;
    int nMinItems_;
    int nMaxItems_;

    ObjectKnowledge* pPopupKnowledge_;
    //@}
};

#endif // __ParamObjectKnowledgeList_h_
