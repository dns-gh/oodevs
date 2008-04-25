// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineListView_h_
#define __TimelineListView_h_

#include <qlistview.h>
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Action_ABC;

// =============================================================================
/** @class  TimelineListView
    @brief  TimelineListView
*/
// Created: SBO 2008-04-22
// =============================================================================
class TimelineListView : public QListView
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< Action_ABC >
                       , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~TimelineListView();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineListView( const TimelineListView& );            //!< Copy constructor
    TimelineListView& operator=( const TimelineListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Action_ABC& action );
    virtual void NotifyDeleted( const Action_ABC& action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual void setContentsPos( int x, int y );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __TimelineListView_h_
