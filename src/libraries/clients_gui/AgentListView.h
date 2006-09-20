// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_AgentListView_h_
#define __Gui_AgentListView_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Hierarchies;
    class Entity_ABC;
}

namespace gui
{
    class ValuedListItem;
    class ItemFactory_ABC;

// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public ListView< AgentListView >
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::Hierarchies >
                    , public kernel::SelectionObserver< kernel::Entity_ABC >
                    , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                    , public kernel::OptionsObserver_ABC
{
   Q_OBJECT;

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentListView();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, ValuedListItem* item );
    virtual void Display( const kernel::Hierarchies& hierarchy, ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentListView( const AgentListView& );
    AgentListView& operator=( const AgentListView& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Hierarchies& hierarchy );
    virtual void NotifyUpdated( const kernel::Hierarchies& hierarchy );
    virtual void NotifyDeleted( const kernel::Hierarchies& hierarchy );
    //@}

    //! @name Helpers
    //@{
private:
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyActivated( const kernel::Entity_ABC& element );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    virtual QDragObject* dragObject();
    virtual void dropEvent( QDropEvent* pEvent );
    virtual void dragEnterEvent( QDragEnterEvent* pEvent );

    bool Drop( ValuedListItem& item, ValuedListItem& target );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Team_ABC* currentTeam_;
    //@}
};

}

#endif // __Gui_AgentListView_h_
