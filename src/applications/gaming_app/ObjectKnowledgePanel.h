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
#include "gaming/TeamSelectionObserver.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Automat_ABC;
    class Team_ABC;
    class Controllers;
    class Displayer_ABC;
    class NBCAttributes_ABC;
    class CampAttributes_ABC;
    class CrossingSiteAttributes_ABC;
    class LogisticRouteAttributes_ABC;
    class RotaAttributes_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class ObjectKnowledges;
class ObjectKnowledge_ABC;
class ObjectKnowledges;
class ObjectPerceptions;

// =============================================================================
/** @class  ObjectKnowledgePanel
    @brief  ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class ObjectKnowledgePanel : public gui::InfoPanel_ABC
                           , public kernel::Observer_ABC
                           , public kernel::ElementObserver_ABC< ObjectKnowledges >
                           , public kernel::ElementObserver_ABC< ObjectKnowledge_ABC >
                           , public kernel::ElementObserver_ABC< ObjectPerceptions >
                           , public kernel::ElementObserver_ABC< kernel::CampAttributes_ABC >
                           , public kernel::ElementObserver_ABC< kernel::CrossingSiteAttributes_ABC >
                           , public kernel::ElementObserver_ABC< kernel::LogisticRouteAttributes_ABC >
                           , public kernel::ElementObserver_ABC< kernel::NBCAttributes_ABC >
                           , public kernel::ElementObserver_ABC< kernel::RotaAttributes_ABC >
                           , public TeamSelectionObserver
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
    void Display( const ObjectKnowledge_ABC& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const kernel::Automat_ABC* agent, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* );
    void ToggleDisplayOwnTeam();
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );
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
    virtual void NotifyUpdated( const ObjectKnowledge_ABC& element );
    virtual void NotifyUpdated( const ObjectPerceptions& element );
    virtual void NotifyUpdated( const kernel::CampAttributes_ABC& element );
    virtual void NotifyUpdated( const kernel::CrossingSiteAttributes_ABC& element );
    virtual void NotifyUpdated( const kernel::LogisticRouteAttributes_ABC& element );
    virtual void NotifyUpdated( const kernel::NBCAttributes_ABC& element );
    virtual void NotifyUpdated( const kernel::RotaAttributes_ABC& element );
    template< typename T >
    void DisplayExtension( const T& extension );
    template< typename T >
    void UpdateExtension( const ObjectKnowledge_ABC& k );
    virtual void Select( const kernel::Team_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    kernel::SafePointer< kernel::Team_ABC > owner_;
    kernel::SafePointer< ObjectKnowledges > selected_;
    gui::ListDisplayer< ObjectKnowledgePanel >* pKnowledgeListView_;

    QCheckBox* pOwnTeamCheckBox_;
    kernel::SafePointer< ObjectKnowledge_ABC > subSelected_;
    gui::DisplayBuilder* display_;

    gui::ListDisplayer< ObjectKnowledgePanel >* pPerceptionListView_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
