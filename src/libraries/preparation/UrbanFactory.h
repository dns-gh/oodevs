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
    class Entity_ABC;
    class Object_ABC;
    class UrbanDisplayOptions;
}

class IdManager;
class StaticModel;

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
             UrbanFactory( kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanObjects, const StaticModel& staticModel,
                           IdManager& idManager, const tools::Resolver< kernel::Object_ABC >& objects, kernel::UrbanDisplayOptions& options );
    virtual ~UrbanFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::UrbanObject_ABC* Create( xml::xistream& xis, kernel::Entity_ABC* parent ) const;
    virtual kernel::UrbanObject_ABC* Create( kernel::Entity_ABC* parent ) const;
    virtual kernel::UrbanObject_ABC* Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                                    controllers_;
    const tools::Resolver_ABC< kernel::UrbanObject_ABC >&   urbanObjects_;
    const StaticModel&                                      staticModel_;
    IdManager&                                              idManager_;
    const tools::Resolver< kernel::Object_ABC >&            objects_;
    kernel::UrbanDisplayOptions&                            options_;
    //@}
};

#endif // __UrbanFactory_h_
