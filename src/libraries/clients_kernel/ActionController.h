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
#include "ActivationObserver_ABC.h"
#include "ContextMenu.h"
#include "ContextMenuObserver_ABC.h"

namespace kernel
{

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

    // $$$$ AGE 2006-08-21: Ajouter des contextMenu sur plusieurs elements ? 
    // $$$$ AGE 2006-08-21: Genre a la fois un point et une entité ? 
    // $$$$ AGE 2006-08-21: Voire meme plusieurs entités ? 
    // $$$$ AGE 2006-08-21: Et se débarasser à moitié du defaultLayer ?
    template< typename T >
    void ContextMenu( const T& element,const QPoint& where )
    {
        menu_.Clear();
        Apply( & ContextMenuObserver_ABC< T >::NotifyContextMenu, element, menu_ );
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

    
private:
    //! @name Member data
    //@{
    bool selecting_;
    ::ContextMenu menu_;
    //@}

};

}

#endif // __ActionController_h_
