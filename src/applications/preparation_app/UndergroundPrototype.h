// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundPrototype_h_
#define __UndergroundPrototype_h_

#include "clients_gui/UndergroundPrototype_ABC.h"

namespace kernel
{
    class Controller;
    class Object_ABC;
}

// =============================================================================
/** @class  UndergroundPrototype
    @brief  UndergroundPrototype
*/
// Created: JSR 2011-07-08
// =============================================================================
class UndergroundPrototype : public gui::UndergroundPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UndergroundPrototype( QWidget* parent, kernel::Controller& controller, kernel::Object_ABC*& creation );
    virtual ~UndergroundPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __UndergroundPrototype_h_
