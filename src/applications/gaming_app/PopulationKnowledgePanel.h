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

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/SafePointer.h"

class Controllers;
class DisplayBuilder;
template< typename T > class ListDisplayer;
class PopulationKnowledges;
class PopulationKnowledge;
class PopulationFlowKnowledge;
class PopulationConcentrationKnowledge;
class KnowledgeGroup_ABC;
class Agent_ABC;
class Displayer_ABC;
class PopulationPartKnowledge_ABC;
class ValuedListItem;
class Team_ABC;
class ItemFactory_ABC;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public InfoPanel_ABC
                               , public Observer_ABC
                               , public KnowledgeGroupSelectionObserver
                               , public ElementObserver_ABC< PopulationKnowledges >
                               , public ElementObserver_ABC< PopulationKnowledge >
                               , public ElementObserver_ABC< PopulationFlowKnowledge >
                               , public ElementObserver_ABC< PopulationConcentrationKnowledge >
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory );
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
    virtual void Select( const KnowledgeGroup_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    SafePointer< Team_ABC > owner_;
    ListDisplayer< PopulationKnowledgePanel >* knowledgeList_;
    DisplayBuilder* display_;
    SafePointer< PopulationKnowledges >        selected_;
    SafePointer< PopulationKnowledge >         subSelected_;
    SafePointer< PopulationPartKnowledge_ABC > selectedPart_; // $$$$ AGE 2006-04-20: won't do

    QCheckBox* pOwnTeamCheckBox_;
    //@}
};

#endif // __PopulationKnowledgePanel_h_
