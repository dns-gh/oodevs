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

class QPopupMenu;

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
            Apply( SelectionObserver_ABC::BeforeSelection );
            Apply( SelectionObserver_Base< T >::Select, element );
            Apply( SelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    template< typename T >
    void ContextMenu( const T& element, QPopupMenu& popupMenu )
    {
        Apply( ContextMenuObserver_ABC< T >::NotifyContextMenu, element, popupMenu );
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActionController( const ActionController& );            //!< Copy constructor
    ActionController& operator=( const ActionController& ); //!< Assignement operator
    //@}

    //! @name Member data
    //@{
    bool selecting_;
    //@}
};

#endif // __ActionController_h_
