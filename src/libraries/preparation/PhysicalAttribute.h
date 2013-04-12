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
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
    class Entity_ABC;
    class ObjectTypes;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
    class xostream;
}

class UrbanObject;

// =============================================================================
/** @class  PhysicalAttribute
    @brief  PhysicalAttribute
*/
// Created: ABR 2012-05-22
// =============================================================================
class PhysicalAttribute : public kernel::PhysicalAttribute_ABC
                        , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PhysicalAttribute( const kernel::Entity_ABC* parent, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                UrbanObject& urbanObject, kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes );
             PhysicalAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                                UrbanObject& urbanObject, kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes );
    virtual ~PhysicalAttribute();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}
};

#endif // __preparation_PhysicalAttribute_h_
