// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePropagationModifierPrototype_h_
#define __FirePropagationModifierPrototype_h_

#include "clients_gui/FirePropagationModifierPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  FirePropagationModifierPrototype
    @brief  FirePropagationModifierPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class FirePropagationModifierPrototype : public gui::FirePropagationModifierPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FirePropagationModifierPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~FirePropagationModifierPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __FirePropagationModifierPrototype_h_
