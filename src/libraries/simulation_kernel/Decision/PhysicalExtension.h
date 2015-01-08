// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_KERNEL_PHYSICAL_EXTENSION_H
#define SIMULATION_KERNEL_PHYSICAL_EXTENSION_H

namespace xml
{
    class xistream;
}

// Reads PhysicalExtensions.xml and stores a key/value map,
// queried by the brain via DEC_GetPhysicalExtension
class PhysicalExtension
{
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static std::string Find( const std::string& key );
    //@}
};

#endif // SIMULATION_KERNEL_PHYSICAL_EXTENSION_H
