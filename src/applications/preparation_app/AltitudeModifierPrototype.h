// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierPrototype_h_
#define __AltitudeModifierPrototype_h_

#include "clients_gui/AltitudeModifierPrototype_ABC.h"

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class Object_ABC;
}

// =============================================================================
/** @class  AltitudeModifierPrototype
    @brief  AltitudeModifierPrototype
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierPrototype : public gui::AltitudeModifierPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AltitudeModifierPrototype( QWidget* parent, kernel::Object_ABC*& creation, kernel::Controllers& controllers, kernel::DetectionMap& detection );
    virtual ~AltitudeModifierPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    kernel::Controllers& controllers_;
    kernel::DetectionMap& detection_;
    //@}
};

#endif // __AltitudeModifierPrototype_h_
