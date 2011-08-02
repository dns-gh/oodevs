// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledgePanel_h_
#define __UrbanKnowledgePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/TeamSelectionObserver.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Displayer_ABC;
    class UrbanKnowledge_ABC;
}

namespace gui
{
    class DisplayBuilder;
    class ItemFactory_ABC;
    template< typename T > class ListDisplayer;
    class ValuedListItem;
}

class UrbanKnowledges;
class UrbanPerceptions;

// =============================================================================
/** @class  UrbanKnowledgePanel
    @brief  UrbanKnowledgePanel
*/
// Created: MGD 2009-12-09
// =============================================================================
class UrbanKnowledgePanel : public gui::InfoPanel_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< UrbanPerceptions >
                          , public tools::ElementObserver_ABC< UrbanKnowledges >
                          , public kernel::TeamSelectionObserver
                          , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~UrbanKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const kernel::UrbanKnowledge_ABC& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const kernel::Automat_ABC* agent, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( Q3ListViewItem* );
    void OnContextMenuRequested( Q3ListViewItem* pItem, const QPoint& pos );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    UrbanKnowledgePanel( const UrbanKnowledgePanel& );
    UrbanKnowledgePanel& operator=( const UrbanKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const UrbanKnowledges& element );
    virtual void NotifyUpdated( const UrbanPerceptions& element );
    virtual void NotifyUpdated( const kernel::UrbanKnowledge_ABC& element );
    virtual void Select( const kernel::Team_ABC* );
    virtual void Select( const kernel::KnowledgeGroup_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    kernel::SafePointer< UrbanKnowledges > selected_;
    gui::ListDisplayer< UrbanKnowledgePanel >* pKnowledgeListView_;

    kernel::SafePointer< kernel::UrbanKnowledge_ABC > subSelected_;
    gui::DisplayBuilder* display_;

    gui::ListDisplayer< UrbanKnowledgePanel >* pPerceptionListView_;
    //@}
};

#endif // __UrbanKnowledgePanel_h_
