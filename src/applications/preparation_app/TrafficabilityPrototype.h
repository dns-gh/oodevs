// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TrafficabilityPrototype_h_
#define __TrafficabilityPrototype_h_

#include "clients_gui/TrafficabilityPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  TrafficabilityPrototype
    @brief  Trafficability prototype
*/
// Created: LGY 2011-08-22
// =============================================================================
class TrafficabilityPrototype : public gui::TrafficabilityPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~TrafficabilityPrototype();
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

#endif // __TrafficabilityPrototype_h_
