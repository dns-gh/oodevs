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

#ifndef __MOS_ParamPopulationKnowledge_h_
#define __MOS_ParamPopulationKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_PopulationKnowledge;
class MOS_Gtia;
class MOS_Agent_ABC;


// =============================================================================
// Created: HME 2005-12-22
// =============================================================================
class MOS_ParamPopulationKnowledge : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamPopulationKnowledge );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamPopulationKnowledge( ASN1T_KnowledgePopulation& asnKnowledge, MOS_Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~MOS_ParamPopulationKnowledge();
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
    void OnPopulationKnowledgeDeleted( MOS_Gtia& gtia, MOS_PopulationKnowledge& knowledge );
    //@}

private:
    std::string strMenuText_;
    ASN1T_KnowledgePopulation& asnKnowledge_;
    MOS_Agent_ABC& agent_;
    MOS_PopulationKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    MOS_PopulationKnowledge* pPopupKnowledge_;
};

#endif // __MOS_ParamPopulationKnowledge_h_
