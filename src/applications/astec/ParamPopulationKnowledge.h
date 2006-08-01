// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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

#ifndef __ParamPopulationKnowledge_h_
#define __ParamPopulationKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class PopulationKnowledge;
class Gtia;
class Agent_ABC;


// =============================================================================
// Created: HME 2005-12-22
// =============================================================================
class ParamPopulationKnowledge : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamPopulationKnowledge );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamPopulationKnowledge( ASN1T_KnowledgePopulation& asnKnowledge, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamPopulationKnowledge();
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
    void OnPopulationKnowledgeDeleted( Gtia& gtia, PopulationKnowledge& knowledge );
    //@}

private:
    std::string strMenuText_;
    ASN1T_KnowledgePopulation& asnKnowledge_;
    Agent_ABC& agent_;
    PopulationKnowledge* pKnowledge_;

    MT_ParameterLabel* pLabel_;
    QLabel* pKnowledgeLabel_;

    PopulationKnowledge* pPopupKnowledge_;
};

#endif // __ParamPopulationKnowledge_h_
