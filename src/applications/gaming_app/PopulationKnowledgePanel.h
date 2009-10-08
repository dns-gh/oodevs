// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
    class PopulationKnowledge_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class PopulationPartKnowledge_ABC;
class PopulationKnowledges;
class PopulationFlowKnowledge;
class PopulationConcentrationKnowledge;

// =============================================================================
// Created: SBO 2005-10-19
// =============================================================================
class PopulationKnowledgePanel : public gui::InfoPanel_ABC
                               , public tools::Observer_ABC
                               , public KnowledgeGroupSelectionObserver
                               , public kernel::ElementObserver_ABC< PopulationKnowledges >
                               , public kernel::ElementObserver_ABC< kernel::PopulationKnowledge_ABC >
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
    void Display( const kernel::PopulationKnowledge_ABC& knowledge, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const PopulationPartKnowledge_ABC& knowledge, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* i );
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
    virtual void NotifyUpdated( const kernel::PopulationKnowledge_ABC& element );
    virtual void NotifyUpdated( const PopulationFlowKnowledge& element );
    virtual void NotifyUpdated( const PopulationConcentrationKnowledge& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ListDisplayer< PopulationKnowledgePanel >* knowledgeList_;
    gui::DisplayBuilder* display_;
    kernel::SafePointer< PopulationKnowledges >            selected_;
    kernel::SafePointer< kernel::PopulationKnowledge_ABC > subSelected_;
    kernel::SafePointer< PopulationPartKnowledge_ABC >     selectedPart_; // $$$$ AGE 2006-04-20: won't do
    //@}
};

#endif // __PopulationKnowledgePanel_h_
