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
    class Team_ABC;
    class Controllers;
    class Displayer_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class DisplayBuilder;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class ObjectKnowledges;
class ObjectKnowledge;
class CampAttributes;
class CrossingSiteAttributes;
class LogisticRouteAttributes;
class NBCAttributes;
class RotaAttributes;
class ObjectKnowledges;

// =============================================================================
/** @class  ObjectKnowledgePanel
    @brief  ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class ObjectKnowledgePanel : public gui::InfoPanel_ABC
                           , public kernel::Observer_ABC
                           , public kernel::ElementObserver_ABC< ObjectKnowledges >
                           , public kernel::ElementObserver_ABC< ObjectKnowledge >
                           , public kernel::ElementObserver_ABC< CampAttributes >
                           , public kernel::ElementObserver_ABC< CrossingSiteAttributes >
                           , public kernel::ElementObserver_ABC< LogisticRouteAttributes >
                           , public kernel::ElementObserver_ABC< NBCAttributes >
                           , public kernel::ElementObserver_ABC< RotaAttributes >
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
    void Display( const ObjectKnowledge& k, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const kernel::Agent_ABC* agent, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
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
    virtual void NotifyUpdated( const ObjectKnowledge& element );
    virtual void NotifyUpdated( const CampAttributes& element );
    virtual void NotifyUpdated( const CrossingSiteAttributes& element );
    virtual void NotifyUpdated( const LogisticRouteAttributes& element );
    virtual void NotifyUpdated( const NBCAttributes& element );
    virtual void NotifyUpdated( const RotaAttributes& element );
    template< typename T >
    void DisplayExtension( const T& extension );
    template< typename T >
    void UpdateExtension( const ObjectKnowledge& k );
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
    kernel::SafePointer< ObjectKnowledge > subSelected_;
    gui::DisplayBuilder* display_;

    gui::ListDisplayer< ObjectKnowledgePanel >* pPerceptionListView_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
