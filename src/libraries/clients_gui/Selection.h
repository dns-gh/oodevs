// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef gui_Selection_h
#define gui_Selection_h

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/GraphicalEntity_ABC.h"
#include <tools/SelectionObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Selection
    @brief  Selection
*/
// Created: LGY 2013-04-05
// =============================================================================
class Selection : public tools::Observer_ABC
                , public tools::SelectionObserver< kernel::Entity_ABC >
                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Selection( kernel::Controllers& controller );
    virtual ~Selection();
    //@}

    //! @name Operations
    //@{
    bool IsSelected( const kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controller_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

}

#endif // gui_Selection_h
