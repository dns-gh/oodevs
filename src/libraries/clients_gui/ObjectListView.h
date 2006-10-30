// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_ObjectListView_h_
#define __gui_ObjectListView_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
    class Team_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectListView
    @brief  ObjectListView
*/
// Created: APE 2004-08-05
// =============================================================================
class ObjectListView : public ListView< ObjectListView >
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< kernel::Object_ABC >
                     , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                     , public kernel::SelectionObserver< kernel::Entity_ABC >
                     , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                     , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectListView();
    //@}

    //! @name Operations
    //@{
     QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnRequestCenter();
    void OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectListView( const ObjectListView& );
    ObjectListView& operator=( const ObjectListView );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Object_ABC& object );
    virtual void NotifyDeleted( const kernel::Object_ABC& object );
    virtual void NotifyUpdated( const kernel::Entity_ABC& object );

    virtual void NotifyActivated( const kernel::Entity_ABC& element );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Team_ABC* currentTeam_; // $$$$ SBO 2006-10-30: profile?
    //@}
};

}

#endif // __gui_ObjectListView_h_
