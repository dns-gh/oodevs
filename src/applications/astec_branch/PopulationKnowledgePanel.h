// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgePanel_h_
#define __PopulationKnowledgePanel_h_

#include "InfoPanel_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "KnowledgeGroupSelectionObserver.h"

class Controller;
class ActionController;
class DisplayBuilder;
template< typename T > class ListDisplayer;
class PopulationKnowledges;
class PopulationKnowledge;
class PopulationFlowKnowledge;
class PopulationConcentrationKnowledge;
class KnowledgeGroup;
class Agent;
class Displayer_ABC;
class PopupulationPart_ABC;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public InfoPanel_ABC
                               , private Observer_ABC
                               , public KnowledgeGroupSelectionObserver
                               , public ElementObserver_ABC< PopulationKnowledges >
                               , public ElementObserver_ABC< PopulationKnowledge >
                               , public ElementObserver_ABC< PopulationFlowKnowledge >
                               , public ElementObserver_ABC< PopulationConcentrationKnowledge >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgePanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~PopulationKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const PopulationKnowledge& part, Displayer_ABC& displayer );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledgePanel( const PopulationKnowledgePanel& );
    PopulationKnowledgePanel& operator=( const PopulationKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const PopulationKnowledges& element );
    virtual void NotifyUpdated( const PopulationKnowledge& element );
    virtual void NotifyUpdated( const PopulationFlowKnowledge& element );
    virtual void NotifyUpdated( const PopulationConcentrationKnowledge& element );
    virtual void Select( const KnowledgeGroup* element );
    //@}

private:
    //! @name Member data
    //@{
    ListDisplayer< PopulationKnowledgePanel >* knowledgeList_;
    DisplayBuilder* display_;
    ListDisplayer< PopulationKnowledgePanel >* perceptionList_;
    const PopulationKnowledges* selected_;
    const PopulationKnowledge* subSelected_; // $$$$ AGE 2006-02-24: Watch for deletions
    const PopupulationPart_ABC* selectedPart_;
//    PopulationKnowledge*              pSelectedKnowledge_;
//    PopulationConcentrationKnowledge* pSelectedConcentrationKnowledge_;
//    PopulationFlowKnowledge*          pSelectedFlowKnowledge_;
//
//    QListView* pKnowledgeListView_;
//    QListView* pPerceptionListView_;
//    QCheckBox* pOwnTeamCheckBox_;
//    

//
//    QGroupBox* pFlowPartBox_;
//    QLabel*    pFlowPartNbr_;
//    QTable*    pFlowPartTable_;
//
//    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __PopulationKnowledgePanel_h_
