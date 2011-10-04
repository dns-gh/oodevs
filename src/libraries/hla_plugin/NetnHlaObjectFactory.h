// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnHlaObjectFactory_h
#define plugins_hla_NetnHlaObjectFactory_h

#include "HlaObjectFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnHlaObjectFactory
    @brief  NETN hla object factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnHlaObjectFactory : public HlaObjectFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnHlaObjectFactory( const HlaObjectFactory_ABC& factory );
    virtual ~NetnHlaObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    const HlaObjectFactory_ABC& factory_;
    //@}
};

}
}

#endif // plugins_hla_NetnHlaObjectFactory_h
