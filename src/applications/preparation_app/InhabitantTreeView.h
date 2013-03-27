// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantTreeView_h_
#define __InhabitantTreeView_h_

#include "clients_gui/EntityTreeView.h"

// =============================================================================
/** @class  InhabitantTreeView
    @brief  InhabitantTreeView
*/
// Created: JSR 2012-09-24
// =============================================================================
class InhabitantTreeView : public gui::InhabitantTreeView
{

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent = 0 );
    virtual ~InhabitantTreeView();
    //@}

private:
    //! @name Operations
    //@{
    virtual void keyPressEvent( QKeyEvent* event );
    //@}
};

#endif // __InhabitantTreeView_h_
