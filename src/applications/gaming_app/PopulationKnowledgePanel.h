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
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>

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
    class DisplayBuilder;
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
                               , public tools::ElementObserver_ABC< PopulationKnowledges >
                               , public tools::ElementObserver_ABC< kernel::PopulationKnowledge_ABC >
                               , public tools::ElementObserver_ABC< PopulationFlowKnowledge >
                               , public tools::ElementObserver_ABC< PopulationConcentrationKnowledge >
                               , public tools::ElementObserver_ABC< kernel::Population_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~PopulationKnowledgePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnContextMenuRequested( const QPoint & pos );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const PopulationKnowledges& element );
    virtual void NotifyUpdated( const kernel::PopulationKnowledge_ABC& element );
    virtual void NotifyUpdated( const PopulationFlowKnowledge& element );
    virtual void NotifyUpdated( const PopulationConcentrationKnowledge& element );
    virtual void NotifyUpdated( const kernel::Population_ABC& population );
    virtual void Select( const kernel::KnowledgeGroup_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QTreeView* knowledgeList_;
    QStandardItemModel knowledgeModel_;
    gui::DisplayBuilder* display_;
    kernel::SafePointer< PopulationKnowledges >            selected_;
    kernel::SafePointer< kernel::PopulationKnowledge_ABC > subSelected_;
    kernel::SafePointer< PopulationPartKnowledge_ABC >     selectedPart_; // $$$$ AGE 2006-04-20: won't do
    //@}
};

#endif // __PopulationKnowledgePanel_h_
