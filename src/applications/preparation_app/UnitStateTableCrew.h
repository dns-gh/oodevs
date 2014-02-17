// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableCrew_h_
#define __UnitStateTableCrew_h_

#include "clients_gui/UnitStateTableCrew.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  UnitStateTableCrew
    @brief  UnitStateTableCrew
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableCrew : public gui::UnitStateTableCrew
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableCrew( QWidget* parent, kernel::Controllers& controllers );
    virtual ~UnitStateTableCrew();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    virtual void Load( kernel::Entity_ABC& selected );
    virtual void Commit( kernel::Entity_ABC& selected ) const;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const;
    //@}
};

#endif // __UnitStateTableCrew_h_
