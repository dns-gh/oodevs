// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFactory_h_
#define __UrbanFactory_h_

#include "UrbanFactory_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controllers;
    class AccommodationTypes;
    class CoordinateConverter_ABC;
    class Object_ABC;
    class ObjectTypes;
    class PropertiesDictionary;
}

namespace gui
{
    class UrbanDisplayOptions;
}

// =============================================================================
/** @class  UrbanFactory
    @brief  Urban factory
*/
// Created: LGY 2012-04-10
// =============================================================================
class UrbanFactory : public UrbanFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanFactory( kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes,
                           const tools::Resolver< kernel::Object_ABC >& objects, gui::UrbanDisplayOptions& options,
                           const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanObjects,
                           const kernel::AccommodationTypes& accommodations, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UrbanFactory();
    //@}

    //! @name Operations
    //@{
    virtual gui::TerrainObjectProxy* Create( xml::xistream& xis ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ObjectTypes& objectTypes_;
    const tools::Resolver< kernel::Object_ABC >& objects_;
    gui::UrbanDisplayOptions& options_;
    const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanObjects_;
    const kernel::AccommodationTypes& accommodations_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __UrbanFactory_h_
