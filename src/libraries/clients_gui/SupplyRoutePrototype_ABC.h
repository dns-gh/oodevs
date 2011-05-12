// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRoutePrototype_ABC_h_
#define __SupplyRoutePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    class LoadableSpinBox;
    class LoadableCheckBox;

// =============================================================================
/** @class  SupplyRoutePrototype_ABC
    @brief  Logistic route prototype definition
*/
// Created: SBO 2006-04-20
// =============================================================================
class SupplyRoutePrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRoutePrototype_ABC( QWidget* parent );
    virtual ~SupplyRoutePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}
protected:
    //! @name Member data
    //@{
    LoadableSpinBox* flow_;
    LoadableSpinBox* width_;
    LoadableSpinBox* length_;
    LoadableSpinBox* maxWeight_;
    LoadableCheckBox* equipped_;
    //@}
};

}

#endif // __SupplyRoutePrototype_ABC_h_
