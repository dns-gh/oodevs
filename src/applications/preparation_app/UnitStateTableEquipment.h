// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableEquipment_h_
#define __UnitStateTableEquipment_h_

#include "clients_gui/UnitStateTableEquipment.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

// =============================================================================
/** @class  UnitStateTableEquipment
    @brief  UnitStateTableEquipment
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableEquipment : public gui::UnitStateTableEquipment
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableEquipment( QWidget* parent, gui::DisplayExtractor& extractor, kernel::Controllers& controllers );
    virtual ~UnitStateTableEquipment();
    //@}

    //! @name Operations
    //@{
    virtual void Load( kernel::Entity_ABC& selected );
    virtual void Commit( kernel::Entity_ABC& selected ) const;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const;
    //@}
};

#endif // __UnitStateTableEquipment_h_
