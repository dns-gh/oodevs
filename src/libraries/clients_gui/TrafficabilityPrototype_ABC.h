// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TrafficabilityPrototype_ABC_h_
#define __TrafficabilityPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    class LoadableLineEdit;
// =============================================================================
/** @class  TrafficabilityPrototype_ABC
    @brief  Trafficability prototype declaration
*/
// Created: LGY 2011-08-23
// =============================================================================
class TrafficabilityPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TrafficabilityPrototype_ABC( QWidget* parent, double maxWeight );
    virtual ~TrafficabilityPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableLineEdit* maxWeight_;
    //@}
};

} // end namespace gui

#endif // __TrafficabilityPrototype_ABC_h_
