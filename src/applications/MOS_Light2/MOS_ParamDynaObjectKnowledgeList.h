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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamDynaObjectKnowledgeList.h $
// $Author: Ape $
// $Modtime: 5/08/04 17:22 $
// $Revision: 4 $
// $Workfile: MOS_ParamDynaObjectKnowledgeList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamDynaObjectKnowledgeList_h_
#define __MOS_ParamDynaObjectKnowledgeList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_Team;
class MOS_DynaObjectKnowledge;
class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamDynaObjectKnowledgeList
    @brief  MOS_ParamDynaObjectKnowledgeList
    @par    Using example
    @code
    MOS_ParamDynaObjectKnowledgeList;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class MOS_ParamDynaObjectKnowledgeList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamDynaObjectKnowledgeList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamDynaObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent );
    ~MOS_ParamDynaObjectKnowledgeList();
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
    void OnObjectKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListKnowledgeObject& asnListKnowledge_;
    MOS_Agent& agent_;
    ASN1T_OID* pAsnOIDList_;
    int nMinItems_;
    int nMaxItems_;

    MOS_DynaObjectKnowledge* pPopupKnowledge_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamDynaObjectKnowledgeList.inl"
#endif

#endif // __MOS_ParamDynaObjectKnowledgeList_h_
