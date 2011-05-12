// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSitePrototype_ABC_h_
#define __CrossingSitePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    class LoadableSpinBox;
    class LoadableCheckBox;

// =============================================================================
/** @class  CrossingSitePrototype_ABC
    @brief  CrossingSitePrototype_ABC
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSitePrototype_ABC( QWidget* parent );
    virtual ~CrossingSitePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableSpinBox*  width_;
    LoadableSpinBox*  depth_;
    LoadableSpinBox*  speed_;
    LoadableCheckBox* needsConstruction_;
    //@}
};

}

#endif // __CrossingSitePrototype_ABC_h_
