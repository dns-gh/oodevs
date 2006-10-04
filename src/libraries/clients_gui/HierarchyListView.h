// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_HierarchyListView_h_
#define __Gui_HierarchyListView_h_

#include "HierarchyListView_ABC.h"

namespace gui
{

// =============================================================================
/** @class  HierarchyListView
    @brief  Displays the agent list sorted according to a hierarchy
*/
// Created: APE 2004-03-15
// =============================================================================
template< typename H >
class HierarchyListView : public HierarchyListView_ABC
                        , public kernel::ElementObserver_ABC< H >
{

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~HierarchyListView();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const kernel::Hierarchies* RetrieveHierarchy( const kernel::Entity_ABC& entity );
    using HierarchyListView_ABC::NotifyCreated;
    using HierarchyListView_ABC::NotifyUpdated;
    using HierarchyListView_ABC::NotifyDeleted;

    virtual void NotifyCreated( const H& hierarchy );
    virtual void NotifyUpdated( const H& hierarchy );
    virtual void NotifyDeleted( const H& hierarchy );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    HierarchyListView( const HierarchyListView& );
    HierarchyListView& operator=( const HierarchyListView& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

}

#include "HierarchyListView.inl"

#endif // __Gui_HierarchyListView_h_
