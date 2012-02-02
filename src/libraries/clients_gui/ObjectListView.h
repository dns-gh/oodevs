// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui_ObjectListView_h
#define gui_ObjectListView_h

#include "EntityListView.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectListView
    @brief  Object list view
*/
// Created: APE 2004-08-05
// =============================================================================
class ObjectListView : public EntityListView
                     , public tools::ElementObserver_ABC< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~ObjectListView();
    //@}

    //! @name Operations
    //@{
    QSize sizeHint() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Object_ABC& object );
    virtual void NotifyDeleted( const kernel::Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    //@}
};

}

#endif // gui_ObjectListView_h
