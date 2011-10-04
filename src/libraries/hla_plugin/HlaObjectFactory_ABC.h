// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaObjectFactory_ABC_h
#define plugins_hla_HlaObjectFactory_ABC_h

#include "rpr/ForceIdentifier.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <memory>

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class HlaObject_ABC;
    class Agent_ABC;

// =============================================================================
/** @class  HlaObjectFactory_ABC
    @brief  Hla object factory definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class HlaObjectFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaObjectFactory_ABC() {}
    virtual ~HlaObjectFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const = 0;
    //@}
};

}
}

#endif // __HlaObjectFactory_ABC_h_
