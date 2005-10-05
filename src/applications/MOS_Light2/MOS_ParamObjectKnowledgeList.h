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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObjectKnowledgeList.h $
// $Author: Ape $
// $Modtime: 5/08/04 17:22 $
// $Revision: 4 $
// $Workfile: MOS_ParamObjectKnowledgeList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamObjectKnowledgeList_h_
#define __MOS_ParamObjectKnowledgeList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_Team;
class MOS_ObjectKnowledge;
class MOS_Agent_ABC;
class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamObjectKnowledgeList
    @brief  MOS_ParamObjectKnowledgeList
    @par    Using example
    @code
    MOS_ParamObjectKnowledgeList;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class MOS_ParamObjectKnowledgeList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamObjectKnowledgeList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListKnowledge, MOS_Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent );
    ~MOS_ParamObjectKnowledgeList();
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
    void OnObjectKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListKnowledgeObject& asnListKnowledge_;
    MOS_Agent_ABC& agent_;
    ASN1T_OID* pAsnOIDList_;
    int nMinItems_;
    int nMaxItems_;

    MOS_ObjectKnowledge* pPopupKnowledge_;
    //@}
};

#   include "MOS_ParamObjectKnowledgeList.inl"

#endif // __MOS_ParamObjectKnowledgeList_h_
