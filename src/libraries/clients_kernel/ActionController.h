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
#include "Controllers.h"
#include "MultipleSelectionObserver_ABC.h"
#include "OverFlyingObserver_ABC.h"
#include "Selectable_ABC.h"
#include <boost/noncopyable.hpp>
#include <geometry/Types.h>

class MapLayer_ABC;

namespace kernel
{

// =============================================================================
/** @class  ActionController
    @brief  Action controller
*/
// Created: AGE 2006-02-16
// =============================================================================
class ActionController : private tools::SortedInterfaceContainer< tools::Observer_ABC >
                       , private boost::noncopyable
{
    friend void Controllers::ChangeMode( int newMode );

public:
    //! @name Constructors/Destructor
    //@{
             ActionController();
    virtual ~ActionController();
    //@}

    //! @name Operations
    //@{
    void DeselectAll();
    void Register  ( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< const Selectable_ABC* > T_Selectables;
    typedef T_Selectables::iterator             IT_Selectables;
    typedef T_Selectables::const_iterator      CIT_Selectables;

    typedef std::map< const MapLayer_ABC*, T_Selectables > T_SelectedMap;
    typedef T_SelectedMap::iterator                       IT_SelectedMap;
    typedef T_SelectedMap::const_iterator                CIT_SelectedMap;

    typedef std::vector< const MapLayer_ABC* > T_Layers;
    typedef T_Layers::iterator                IT_Layers;
    typedef T_Layers::const_iterator         CIT_Layers;

    typedef std::map< int, T_Layers >          T_MultipleLayers;
    typedef T_MultipleLayers::iterator        IT_MultipleLayers;
    typedef T_MultipleLayers::const_iterator CIT_MultipleLayers;
    //@}

public:
    //! @name Operations
    //@{
    void AllowLayerMultipleSelection( int mode, const MapLayer_ABC* layer );
    bool IsSingleSelection( const MapLayer_ABC* layer ) const;
    bool HasMultipleLayers() const;

    // -----------------------------------------------------------------------------
    // Select
    // -----------------------------------------------------------------------------
    void SetSelected( const MapLayer_ABC* layer, const Selectable_ABC& selectable, bool append );
    void AddToSelection( const MapLayer_ABC* layer, const T_Selectables& selectables );
    void NotifyRectangleSelection( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight, bool append );

    template< typename T >
    void Select( const T& element )
    {
        // avoid re entrance
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
        // avoid re entrance
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
        // avoid re entrance
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
    void MultipleSelect( const std::vector< const T* >& elements )
    {
        // avoid re entrance
        if( ! selecting_ )
        {
            selecting_ = true;
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
            Apply( & kernel::MultipleSelectionObserver_Base< T >::MultipleSelect, element );
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }
    template< typename T1, typename T2 >
    void MultipleSelect( const std::vector< const T1* >& firstElements, const std::vector< const T2* >& secondElements )
    {
        // avoid re entrance
        if( ! selecting_ )
        {
            selecting_ = true;
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
            Apply( & kernel::MultipleSelectionObserver_Base< T1 >::MultipleSelect, firstElements );
            Apply( & kernel::MultipleSelectionObserver_Base< T2 >::MultipleSelect, secondElements );
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }
    template< typename T1, typename T2, typename T3 >
    void MultipleSelect( const std::vector< const T1* >& firstElements, const std::vector< const T2* >& secondElements, const std::vector< const T3* >& thirdElements )
    {
        // avoid re entrance
        if( ! selecting_ )
        {
            selecting_ = true;
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::BeforeSelection );
            Apply( & kernel::MultipleSelectionObserver_Base< T1 >::MultipleSelect, firstElements );
            Apply( & kernel::MultipleSelectionObserver_Base< T2 >::MultipleSelect, secondElements );
            Apply( & kernel::MultipleSelectionObserver_Base< T3 >::MultipleSelect, thirdElements );
            if( !selectInRectangle_ )
                Apply( & kernel::MultipleSelectionObserver_ABC::AfterSelection );
        }
        selecting_ = false;
    }

    // -----------------------------------------------------------------------------
    // ContextMenu
    // -----------------------------------------------------------------------------
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

    // -----------------------------------------------------------------------------
    // Activate
    // -----------------------------------------------------------------------------
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

    // -----------------------------------------------------------------------------
    // OverFly
    // -----------------------------------------------------------------------------
    template< typename T>
    void OverFly( const T& element )
    {
        // avoid re entrance
        if( !overFlying_ )
        {
            overFlying_ = true;
            Apply( & OverFlyingObserver_ABC::BeforeOverFlying );
            Apply( & OverFlyingObserver_Base< T >::OverFly, element );
            Apply( & OverFlyingObserver_ABC::AfterOverFlying );
        }
        overFlying_ = false;
    }
    template< typename T1, typename T2 >
    void OverFly( const T1& firstElement, const T2& secondElement )
    {
        // avoid re entrance
        if( !overFlying_ )
        {
            overFlying_ = true;
            Apply( & OverFlyingObserver_ABC::BeforeOverFlying );
            Apply( & OverFlyingObserver_Base< T1 >::OverFly, firstElement );
            Apply( & OverFlyingObserver_Base< T2 >::OverFly, secondElement );
            Apply( & OverFlyingObserver_ABC::AfterOverFlying );
        }
        overFlying_ = false;
    }
    //@}

private:
    //! @name Helpers
    //@{
    void ChangeMode( int newMode );
    void ShowMenu( const QPoint& where );
    void ClearSingleSelection();
    void ClearMultipleSelection();
    void CleanSelectedMap();
    //@}

private:
    //! @name Member data
    //@{
    int currentMode_;
    bool selecting_;
    bool overFlying_;
    bool selectInRectangle_;
    kernel::ContextMenu menu_;
    T_MultipleLayers multipleLayers_;
    T_SelectedMap selectedMap_;
    static const T_Selectables emptyList_;
    //@}
};

}

#endif // __ActionController_h_
