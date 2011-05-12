// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayPrototype_ABC_h_
#define __DelayPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui {

    class LoadableTimeEdit;

// =============================================================================
/** @class  DelayPrototype_ABC
    @brief  DelayPrototype_ABC
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DelayPrototype_ABC( QWidget* parent );
    virtual ~DelayPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableTimeEdit* delayTime_;
    //@}
};

}

#endif // __DelayPrototype_ABC_h_
