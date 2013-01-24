// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gaming_PhysicalAttribute_h_
#define __gaming_PhysicalAttribute_h_

#include "clients_kernel/PhysicalAttribute_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class ObjectTypes;
    class PropertiesDictionary;
}

namespace gui
{
    class UrbanObject;
}

namespace sword
{
    class UrbanAttributes;
}

// =============================================================================
/** @class  PhysicalAttribute
    @brief  PhysicalAttribute
*/
// Created: ABR 2012-05-22
// =============================================================================
class PhysicalAttribute : public kernel::PhysicalAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PhysicalAttribute( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                gui::UrbanObject& urbanObject, const kernel::ObjectTypes& objectTypes, kernel::Controller& controller );
    virtual ~PhysicalAttribute();
    //@}
};

#endif // __gaming_PhysicalAttribute_h_
