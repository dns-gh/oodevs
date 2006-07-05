// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionController_h_
#define __ActionController_h_

#include "InterfaceContainer.h"
#include "Observer_ABC.h"
#include "SelectionObserver_ABC.h"
#include "ContextMenuObserver_ABC.h"
#include "ActivationObserver_ABC.h"
#include <qpopupmenu.h>

// =============================================================================
/** @class  ActionController
    @brief  Action controller
*/
// Created: AGE 2006-02-16
// =============================================================================
class ActionController : private InterfaceContainer< Observer_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionController();
    virtual ~ActionController();
    //@}

    //! @name Operations
    //@{
    void Register( Observer_ABC& observer );
    void Remove  ( Observer_ABC& observer );
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Select( const T& element )
    {
        // avoid reentrance
        if( ! selecting_ )
        {
            selecting_ = true;
            Apply( & SelectionObserver_ABC::BeforeSelection );
            Apply( & SelectionObserver_Base< T >::Select, element );
            Apply( & SelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    template< typename T >
    void ContextMenu( const T& element,const QPoint& where )
    {
        popupMenu_->clear();
        Apply( & ContextMenuObserver_ABC< T >::NotifyContextMenu, element, *popupMenu_ );
        ShowMenu( where );
    }

    template< typename T >
    void Activate( const T& element )
    {
        Apply( & ActivationObserver_ABC< T >::NotifyActivated, element );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActionController( const ActionController& );            //!< Copy constructor
    ActionController& operator=( const ActionController& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ShowMenu( const QPoint& where );
    //@}

    //! @name Member data
    //@{
    bool selecting_;
    QPopupMenu* popupMenu_;
    //@}
};

#endif // __ActionController_h_
