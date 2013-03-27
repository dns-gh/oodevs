// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __HierarchyTreeView_h_
#define __HierarchyTreeView_h_

#include "HierarchyTreeView_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
class EntitySymbols;
class ModelObserver_ABC;

// =============================================================================
/** @class  HierarchyTreeView
    @brief  HierarchyTreeView
*/
// Created: ABR 2012-08-10
// =============================================================================
template< typename Hierarchy >
class HierarchyTreeView : public HierarchyTreeView_ABC
                        , public tools::ElementObserver_ABC< Hierarchy >
{

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent = 0 );
    virtual ~HierarchyTreeView();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const Hierarchy& hierarchy );
    virtual void NotifyUpdated( const Hierarchy& hierarchy );
    virtual void NotifyDeleted( const Hierarchy& hierarchy );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#include "HierarchyTreeView.inl"

typedef HierarchyTreeView< kernel::CommunicationHierarchies > CommunicationTreeView;


} //! namespace gui

#endif // __HierarchyTreeView_h_
