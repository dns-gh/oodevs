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
class PopulationPartKnowledge_ABC;
class ValuedListItem;
class Team;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public InfoPanel_ABC
                               , private Observer_ABC
                               , public KnowledgeGroupSelectionObserver
                               , public ElementObserver_ABC< PopulationKnowledges > // $$$$ AGE 2006-03-07: watch for PopulationKnowledge creation / deletion instead
                               , public ElementObserver_ABC< PopulationKnowledge >
                               , public ElementObserver_ABC< PopulationFlowKnowledge >
                               , public ElementObserver_ABC< PopulationConcentrationKnowledge >
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgePanel( InfoPanels* pParent, Controller& controller, ActionController& actionController );
    virtual ~PopulationKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const PopulationKnowledge& knowledge, Displayer_ABC& displayer, ValuedListItem* item );
    void Display( const PopulationPartKnowledge_ABC& knowledge, Displayer_ABC& displayer, ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* i );
    void ToggleDisplayOwnTeam(); // $$$$ AGE 2006-03-13: factoriser tous ces slots classiques
    void OnContextMenuRequested( QListViewItem*, const QPoint& );
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
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const PopulationKnowledges& element );
    virtual void NotifyUpdated( const PopulationKnowledge& element );
    virtual void NotifyUpdated( const PopulationFlowKnowledge& element );
    virtual void NotifyUpdated( const PopulationConcentrationKnowledge& element );
    virtual void Select( const KnowledgeGroup* element );
    //@}

private:
    //! @name Member data
    //@{
    ActionController& actionController_;
    const Team* owner_;
    ListDisplayer< PopulationKnowledgePanel >* knowledgeList_;
    DisplayBuilder* display_;
    const PopulationKnowledges* selected_;
    const PopulationKnowledge* subSelected_;
    const PopulationPartKnowledge_ABC* selectedPart_;

    QCheckBox* pOwnTeamCheckBox_;
    //@}
};

#endif // __PopulationKnowledgePanel_h_
