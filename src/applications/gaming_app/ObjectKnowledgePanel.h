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
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Team_ABC;
    class Controllers;
    class Displayer_ABC;
    class ModelUnLoaded;
    class ObjectKnowledge_ABC;
    class ConstructionAttribute_ABC;
    class ObstacleAttribute_ABC;
    class BypassAttribute_ABC;
    class MineAttribute_ABC;
    class NBCAttribute_ABC;
    class LogisticAttribute_ABC;
    class CrossingSiteAttribute_ABC;
    class SupplyRouteAttribute_ABC;
    class TimeLimitedAttribute_ABC;
    class Object_ABC;
}

namespace gui
{
    class DisplayBuilder;
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
                           , public tools::ElementObserver_ABC< kernel::ConstructionAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::MineAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::ObstacleAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::BypassAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::LogisticAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::CrossingSiteAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::SupplyRouteAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::NBCAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::TimeLimitedAttribute_ABC >
                           , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                           , public tools::ElementObserver_ABC< kernel::ObjectKnowledge_ABC >
                           , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                           , public tools::ElementObserver_ABC< kernel::Object_ABC >
                           , public kernel::TeamSelectionObserver
                           , public KnowledgeGroupSelectionObserver
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ObjectKnowledgePanel();
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
    virtual void NotifyUpdated( const kernel::TimeLimitedAttribute_ABC& element );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyUpdated( const kernel::Automat_ABC& automat );
    virtual void NotifyUpdated( const kernel::Object_ABC& object );

    template< typename T >
    void DisplayExtension( const T& extension );
    template< typename T >
    void UpdateExtension( const kernel::ObjectKnowledge_ABC& k );
    virtual void Select( const kernel::Team_ABC* );
    virtual void Select( const kernel::KnowledgeGroup_ABC* );
    void Select( const ObjectKnowledges* );
    void UpdateAllExtensions();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    kernel::SafePointer< ObjectKnowledges > selected_;
    QTreeView* pKnowledgeListView_;
    QStandardItemModel knowledgeModel_;

    kernel::SafePointer< kernel::ObjectKnowledge_ABC > subSelected_;
    gui::DisplayBuilder* display_;

    QTreeView* pPerceptionListView_;
    QStandardItemModel perceptionModel_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
