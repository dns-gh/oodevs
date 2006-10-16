// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectListView_h_
#define __ObjectListView_h_

#include "clients_gui/ObjectListView.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

// =============================================================================
/** @class  ObjectListView
    @brief  ObjectListView
*/
// Created: SBO 2006-10-16
// =============================================================================
class ObjectListView : public gui::ObjectListView
                     , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ObjectListView();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectListView( const ObjectListView& );            //!< Copy constructor
    ObjectListView& operator=( const ObjectListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC&, kernel::ContextMenu& );
    virtual void keyPressEvent( QKeyEvent* key );
    //@}
};

#endif // __ObjectListView_h_
