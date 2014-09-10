// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_ObjectTreeView_h_
#define __gui_ObjectTreeView_h_

#include "EntityTreeView_ABC.h"

namespace kernel
{
    class Object_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ObjectTreeView
    @brief  ObjectTreeView
*/
// Created: JSR 2012-09-13
// =============================================================================
class ObjectTreeView : public EntityTreeView_ABC
                     , public tools::ElementObserver_ABC< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, QWidget* parent = 0 );
    virtual ~ObjectTreeView();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Object_ABC& object );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QStringList MimeTypes() const;
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    //@}
};

}

#endif // __gui_ObjectTreeView_h_
