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

namespace kernel
{
    class Controllers;
    class KnowledgeGroup_ABC;
    class Agent_ABC;
    class Displayer_ABC;
    class Entity_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class PopulationKnowledges;
class PopulationKnowledge_ABC;
class PopulationFlowKnowledge;
class PopulationConcentrationKnowledge;
class PopulationPartKnowledge_ABC;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public gui::InfoPanel_ABC
                               , public kernel::Observer_ABC
                               , public KnowledgeGroupSelectionObserver
                               , public kernel::ElementObserver_ABC< PopulationKnowledges >
                               , public kernel::ElementObserver_ABC< PopulationKnowledge_ABC >
                               , public kernel::ElementObserver_ABC< PopulationFlowKnowledge >
                               , public kernel::ElementObserver_ABC< PopulationConcentrationKnowledge >
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~PopulationKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const PopulationKnowledge_ABC& knowledge, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const PopulationPartKnowledge_ABC& knowledge, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
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
    virtual void NotifyUpdated( const PopulationKnowledge_ABC& element );
    virtual void NotifyUpdated( const PopulationFlowKnowledge& element );
    virtual void NotifyUpdated( const PopulationConcentrationKnowledge& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > owner_;
    gui::ListDisplayer< PopulationKnowledgePanel >* knowledgeList_;
    gui::DisplayBuilder* display_;
    kernel::SafePointer< PopulationKnowledges >        selected_;
    kernel::SafePointer< PopulationKnowledge_ABC >     subSelected_;
    kernel::SafePointer< PopulationPartKnowledge_ABC > selectedPart_; // $$$$ AGE 2006-04-20: won't do

    QCheckBox* pOwnTeamCheckBox_;
    //@}
};

#endif // __PopulationKnowledgePanel_h_
