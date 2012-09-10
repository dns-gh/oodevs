// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_TacticalTreeView_h_
#define __gui_TacticalTreeView_h_

#include "HierarchyTreeView.h"
#include "clients_kernel/TacticalHierarchies.h"

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
/** @class  TacticalTreeView
    @brief  TacticalTreeView
*/
// Created: JSR 2012-08-31
// =============================================================================
class TacticalTreeView : public HierarchyTreeView< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent = 0 );
    virtual ~TacticalTreeView();
    //@}

public:
    //! @name Filters/Sort
    //@{
    virtual bool LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const;
    //@}
};

}

#endif // __gui_TacticalTreeView_h_
