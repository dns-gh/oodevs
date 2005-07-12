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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamDynaObjectKnowledge.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 5 $
// $Workfile: MOS_ParamDynaObjectKnowledge.h $
//
// *****************************************************************************

#ifndef __MOS_ParamDynaObjectKnowledge_h_
#define __MOS_ParamDynaObjectKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_DynaObjectKnowledge;
class MOS_Team;
class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamDynaObjectKnowledge
    @brief  MOS_ParamDynaObjectKnowledge
    @par    Using example
    @code
    MOS_ParamDynaObjectKnowledge;
    @endcode
*/
// Created: APE 2004-05-10
// =============================================================================
class MOS_ParamDynaObjectKnowledge : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamDynaObjectKnowledge );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamDynaObjectKnowledge( ASN1T_KnowledgeObject& asnKnowledge, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent );
    ~MOS_ParamDynaObjectKnowledge();
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
    std::string strMenuText_;
    ASN1T_KnowledgeObject& asnKnowledge_;
    MOS_Agent& agent_;
    MOS_DynaObjectKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    MOS_DynaObjectKnowledge* pPopupKnowledge_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamDynaObjectKnowledge.inl"
#endif

#endif // __MOS_ParamDynaObjectKnowledge_h_
