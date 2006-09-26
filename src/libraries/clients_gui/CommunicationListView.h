// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_CommunicationListView_h_
#define __Gui_CommunicationListView_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class CommunicationHierarchies;
    class Entity_ABC;
}

namespace gui
{
    class ValuedListItem;
    class ItemFactory_ABC;

// =============================================================================
/** @class  CommunicationListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class CommunicationListView : public ListView< CommunicationListView >
                            , public kernel::Observer_ABC
                            , public kernel::ElementObserver_ABC< kernel::CommunicationHierarchies >
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
             CommunicationListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~CommunicationListView();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, ValuedListItem* item );
    virtual void Display( const kernel::CommunicationHierarchies& hierarchy, ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    CommunicationListView( const CommunicationListView& );
    CommunicationListView& operator=( const CommunicationListView& );
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
    virtual void NotifyCreated( const kernel::CommunicationHierarchies& hierarchy );
    virtual void NotifyUpdated( const kernel::CommunicationHierarchies& hierarchy );
    virtual void NotifyDeleted( const kernel::CommunicationHierarchies& hierarchy );
    //@}

private:
    //! @name Helpers
    //@{
    void RecursiveCreateHierarchy( const kernel::CommunicationHierarchies* hierarchy );
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

#endif // __Gui_CommunicationListView_h_
