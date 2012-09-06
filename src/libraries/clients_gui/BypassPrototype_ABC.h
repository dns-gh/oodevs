// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BypassPrototype_ABC_h_
#define __BypassPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    class LoadableSpinBox;

// =============================================================================
/** @class  BypassPrototype_ABC
    @brief  BypassPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class BypassPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BypassPrototype_ABC( QWidget* parent );
    virtual ~BypassPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity( const kernel::Team_ABC& team ) const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableSpinBox* bypassConstructionPercentage_;
    //@}
};

}

#endif // __BypassPrototype_ABC_h_
