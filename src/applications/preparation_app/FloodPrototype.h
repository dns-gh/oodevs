// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodPrototype_h_
#define __FloodPrototype_h_

#include "clients_gui/FloodPrototype_ABC.h"

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class Object_ABC;
}

// =============================================================================
/** @class  FloodPrototype
    @brief  FloodPrototype
*/
// Created: JSR 2010-12-07
// =============================================================================
class FloodPrototype : public gui::FloodPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodPrototype( QWidget* parent, kernel::Object_ABC*& creation, kernel::Controllers& controllers, const kernel::DetectionMap& detection );
    virtual ~FloodPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    //@}
};

#endif // __FloodPrototype_h_
