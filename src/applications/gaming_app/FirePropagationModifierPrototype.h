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

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  FirePropagationModifierPrototype
     @see    4311340-Modèles de feu.doc
*/
// Created: BCI 2011-01-11
// =============================================================================
class FirePropagationModifierPrototype : public gui::FirePropagationModifierPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    FirePropagationModifierPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~FirePropagationModifierPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __FirePropagationModifierPrototype_h_
