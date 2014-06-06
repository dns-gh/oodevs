// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Pathfind_h_
#define __Pathfind_h_

#include <clients_gui/EntityImplementation.h>
#include <clients_kernel/Pathfind_ABC.h>

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Note
    @brief  Note
*/
// =============================================================================
class Pathfind : public gui::EntityImplementation< kernel::Pathfind_ABC >
               , public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Pathfind( kernel::Controller& controller, const sword::Pathfind& msg );
    virtual ~Pathfind();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::unique_ptr< sword::Pathfind > data_;
    //@}

};

#endif // __Note_h_
