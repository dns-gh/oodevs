// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaObjectFactory_h
#define plugins_hla_HlaObjectFactory_h

#include "HlaObjectFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  HlaObjectFactory
    @brief  Hla object factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class HlaObjectFactory : public HlaObjectFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaObjectFactory();
    virtual ~HlaObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const;
    //@}
};

}
}

#endif // plugins_hla_HlaObjectFactory_h
