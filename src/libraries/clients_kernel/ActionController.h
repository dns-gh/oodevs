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

#include "tools/SortedInterfaceContainer.h"
#include "tools/Observer_ABC.h"
#include "tools/SelectionObserver_ABC.h"
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
class ActionController : private tools::SortedInterfaceContainer< tools::Observer_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionController();
    virtual ~ActionController();
    //@}

    //! @name Operations
    //@{
    void Register  ( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );
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
            Apply( & tools::SelectionObserver_ABC::BeforeSelection );
            Apply( & tools::SelectionObserver_Base< T >::Select, element );
            Apply( & tools::SelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    template< typename T1, typename T2 >
    void Select( const T1& firstElement, const T2& secondElement )
    {
        // avoid reentrance
        if( ! selecting_ )
        {
            selecting_ = true;
            Apply( & tools::SelectionObserver_ABC::BeforeSelection );
            Apply( & tools::SelectionObserver_Base< T1 >::Select, firstElement );
            Apply( & tools::SelectionObserver_Base< T2 >::Select, secondElement );
            Apply( & tools::SelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    template< typename T1, typename T2, typename T3 >
    void Select( const T1& firstElement, const T2& secondElement, const T3& thirdElement )
    {
        // avoid reentrance
        if( ! selecting_ )
        {
            selecting_ = true;
            Apply( & tools::SelectionObserver_ABC::BeforeSelection );
            Apply( & tools::SelectionObserver_Base< T1 >::Select, firstElement );
            Apply( & tools::SelectionObserver_Base< T2 >::Select, secondElement );
            Apply( & tools::SelectionObserver_Base< T3 >::Select, thirdElement );
            Apply( & tools::SelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    template< typename T >
    void ContextMenu( const T& element, const QPoint& where )
    {
        menu_.Clear();
        Apply( & ContextMenuObserver_ABC< T >::NotifyContextMenu, element, menu_ );
        ShowMenu( where );
    }

    template< typename T1, typename T2 >
    void ContextMenu( const T1& firstElement, const T2& secondElement, const QPoint& where )
    {
        menu_.Clear();
        Apply( & ContextMenuObserver_ABC< T1 >::NotifyContextMenu, firstElement, menu_ );
        Apply( & ContextMenuObserver_ABC< T2 >::NotifyContextMenu, secondElement, menu_ );
        ShowMenu( where );
    }

    template< typename T1, typename T2, typename T3 >
    void ContextMenu( const T1& firstElement, const T2& secondElement, const T3& thirdElement, const QPoint& where )
    {
        menu_.Clear();
        Apply( & ContextMenuObserver_ABC< T1 >::NotifyContextMenu, firstElement, menu_ );
        Apply( & ContextMenuObserver_ABC< T2 >::NotifyContextMenu, secondElement, menu_ );
        Apply( & ContextMenuObserver_ABC< T3 >::NotifyContextMenu, thirdElement, menu_ );
        ShowMenu( where );
    }

    template< typename T >
    void Activate( const T& element )
    {
        Apply( & ActivationObserver_ABC< T >::NotifyActivated, element );
    }

    template< typename T1, typename T2 >
    void Activate( const T1& firstElement, const T2& secondElement )
    {
        Apply( & ActivationObserver_ABC< T1 >::NotifyActivated, firstElement );
        Apply( & ActivationObserver_ABC< T2 >::NotifyActivated, secondElement );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionController( const ActionController& );            //!< Copy constructor
    ActionController& operator=( const ActionController& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ShowMenu( const QPoint& where );
    //@}

private:
    //! @name Member data
    //@{
    bool selecting_;
    kernel::ContextMenu menu_;
    //@}

};

}

#endif // __ActionController_h_
