// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierPrototype_ABC_h_
#define __AltitudeModifierPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
class LoadableSpinBox;

// =============================================================================
/** @class  AltitudeModifierPrototype_ABC
    @brief  AltitudeModifierPrototype_ABC
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AltitudeModifierPrototype_ABC( QWidget* parent );
    virtual ~AltitudeModifierPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableSpinBox* height_;
    //@}
};

}

#endif // __AltitudeModifierPrototype_ABC_h_
