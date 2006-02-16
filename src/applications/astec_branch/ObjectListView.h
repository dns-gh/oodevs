// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.h $
// $Author: Ape $
// $Modtime: 26/08/04 11:03 $
// $Revision: 3 $
// $Workfile: ObjectListView.h $
//
// *****************************************************************************

#ifndef __ObjectListView_h_
#define __ObjectListView_h_

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"

class Object_ABC;
class Controller;


// =============================================================================
/** @class  ObjectListView
    @brief  ObjectListView
*/
// Created: APE 2004-08-05
// =============================================================================
class ObjectListView : public QListView
                     , public Observer_ABC
                     , public ElementObserver_ABC< Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, Controller& controller );
    virtual ~ObjectListView();
    //@}

    //! @name Operations
    //@{
     QSize sizeHint() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectListView( const ObjectListView& );
    ObjectListView& operator=( const ObjectListView );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Object_ABC& object );
    virtual void NotifyUpdated( const Object_ABC& object );
    virtual void NotifyDeleted( const Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __ObjectListView_h_
