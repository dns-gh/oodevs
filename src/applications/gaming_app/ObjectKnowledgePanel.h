// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgePanel_h_
#define __ObjectKnowledgePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/TeamSelectionObserver.h"
#include "gaming/KnowledgeGroupSelectionObserver.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Automat_ABC;
    class Team_ABC;
    class Controllers;
    class Displayer_ABC;

    class ObjectKnowledge_ABC;
    class ConstructionAttribute_ABC;
    class ObstacleAttribute_ABC;
    class BypassAttribute_ABC;
    class MineAttribute_ABC;
    class NBCAttribute_ABC;
    class LogisticAttribute_ABC;
    class CrossingSiteAttribute_ABC;
    class SupplyRouteAttribute_ABC;
    class ActivityTimeAttribute_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class ObjectKnowledges;
class ObjectPerceptions;

// =============================================================================
/** @class  ObjectKnowledgePanel
    @brief  ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class ObjectKnowledgePanel : public gui::InfoPanel_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< ObjectKnowledges >
                           , public tools::ElementObserver_ABC< ObjectPerceptions >
                           , public tools::ElementObserver_ABC< kernel::ObjectKnowledge_ABC >
                           , public tools::ElementObserver_ABC< kernel::ConstructionAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::MineAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::ObstacleAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::BypassAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::LogisticAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::CrossingSiteAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::SupplyRouteAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::NBCAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::ActivityTimeAttribute_ABC >
                           , public kernel::TeamSelectionObserver
                           , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ObjectKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const kernel::ObjectKnowledge_ABC& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
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
    ObjectKnowledgePanel( const ObjectKnowledgePanel& );
    ObjectKnowledgePanel& operator=( const ObjectKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const ObjectKnowledges& element );
    virtual void NotifyUpdated( const ObjectPerceptions& element );
    virtual void NotifyUpdated( const kernel::ObjectKnowledge_ABC& element );
    virtual void NotifyUpdated( const kernel::ConstructionAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::BypassAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::ObstacleAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::MineAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::LogisticAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::CrossingSiteAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::SupplyRouteAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::NBCAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::ActivityTimeAttribute_ABC& element );
    template< typename T >
    void DisplayExtension( const T& extension );
    template< typename T >
    void UpdateExtension( const kernel::ObjectKnowledge_ABC& k );
    virtual void Select( const kernel::Team_ABC* );
    virtual void Select( const kernel::KnowledgeGroup_ABC* );
    void Select( const ObjectKnowledges* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    kernel::SafePointer< ObjectKnowledges > selected_;
    gui::ListDisplayer< ObjectKnowledgePanel >* pKnowledgeListView_;

    kernel::SafePointer< kernel::ObjectKnowledge_ABC > subSelected_;
    gui::DisplayBuilder* display_;

    gui::ListDisplayer< ObjectKnowledgePanel >* pPerceptionListView_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
