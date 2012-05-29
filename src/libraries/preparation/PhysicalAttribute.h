// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __preparation_PhysicalAttribute_h_
#define __preparation_PhysicalAttribute_h_

#include "clients_kernel/PhysicalAttribute_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

class UrbanObject;

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
             PhysicalAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                UrbanObject& urbanObject, kernel::Controller& controller );
    virtual ~PhysicalAttribute();
    //@}
};

#endif // __preparation_PhysicalAttribute_h_
