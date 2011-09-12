// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePropagationModifierPrototype_ABC_h_
#define __FirePropagationModifierPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{

// =============================================================================
/** @class  FirePropagationModifierPrototype_ABC
    @brief  FirePropagationModifierPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class FirePropagationModifierPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    FirePropagationModifierPrototype_ABC( QWidget* parent );
    virtual ~FirePropagationModifierPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}
};

}

#endif // __FirePropagationModifierPrototype_ABC_h_
