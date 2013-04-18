// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_HierarchyListView_ABC_h_
#define __Gui_HierarchyListView_ABC_h_

#include "ListView.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class AgentType;
    class AutomatType;
    class Controllers;
    class Ghost_ABC;
    class Hierarchies;
    class Entity_ABC;
    class Profile_ABC;
    class Symbol_ABC;
}

enum E_GhostType;

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  HierarchyListView_ABC
    @brief  Displays the agent list sorted according to a hierarchy
*/
// Created: APE 2004-03-15
// =============================================================================
class HierarchyListView_ABC : public ListView< HierarchyListView_ABC >
                            , public tools::Observer_ABC
                            , public tools::SelectionObserver< kernel::Entity_ABC >
                            , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                            , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                            , public tools::ElementObserver_ABC< kernel::Symbol_ABC >
{
   Q_OBJECT

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyListView_ABC( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, const EntitySymbols& symbols );
    virtual ~HierarchyListView_ABC();
    //@}

    //! @name Operations
    //@{
    QSize sizeHint() const;
    virtual void Display( const kernel::Entity_ABC& entity, ValuedListItem* item );

    void ActivateSelection( bool activate );
    //@}

signals:
    void ListViewFocusIn( gui::HierarchyListView_ABC* );

protected slots:
    //! @name Slots
    //@{
    virtual void OnSelectionChange();
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    void OnRequestCenter();
    void Update();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const kernel::Hierarchies* RetrieveHierarchy( const kernel::Entity_ABC& entity ) const = 0;
    virtual void NotifyCreated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyUpdated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyDeleted( const kernel::Hierarchies& hierarchies );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const kernel::Symbol_ABC& symbol );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyActivated( const kernel::Entity_ABC& element );
    virtual void DisplayIcon( const kernel::Entity_ABC& entity, ValuedListItem* item );
    virtual void ClearSelection();
    static void SetVisible( Q3ListViewItem* item, bool visible );
    void UpdateItem( ValuedListItem* item );
    bool IsActivated() const;
    virtual void focusInEvent( QFocusEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    ValuedListItem* FindOrCreate( const kernel::Entity_ABC* entity );
    virtual Q3DragObject* dragObject();
    virtual void viewportDropEvent( QDropEvent* pEvent );
    virtual void viewportDragMoveEvent( QDragMoveEvent *pEvent );
    virtual void viewportDragEnterEvent( QDragEnterEvent* pEvent );
    virtual bool Drop( const kernel::Entity_ABC& entity, ValuedListItem& target );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::AgentType& item, kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::AutomatType& item, kernel::Entity_ABC& target );
    template< typename T>
    bool Drop( const T& type, ValuedListItem& target );

    virtual bool CanDrop( const kernel::Entity_ABC* entity, QPoint position ) const;
    template< typename T >
    bool CanDropOnGhost( const T* type, QPoint position, E_GhostType ghostType ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers&                      controllers_;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Profile_ABC&                profile_;
    const EntitySymbols&                      symbols_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QTimer*                                   timer_;
    QPoint                                    dragStartPosition_;
    bool                                      activated_;
    //@}
};

}

#endif // __Gui_HierarchyListView_ABC_h_