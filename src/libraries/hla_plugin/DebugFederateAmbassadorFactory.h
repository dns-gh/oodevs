// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_DebugFederateAmbassadorFactory_h
#define plugins_hla_DebugFederateAmbassadorFactory_h

#include "FederateAmbassadorFactory_ABC.h"

namespace dispatcher
{
    class Logger_ABC;
}

namespace hla
{
    class FederateAmbassador_ABC;
}

namespace plugins
{
namespace hla
{
    class ObjectResolver_ABC;

// =============================================================================
/** @class  DebugFederateAmbassadorFactory
    @brief  Debug federate ambassador factory
*/
// Created: SLI 2011-05-27
// =============================================================================
class DebugFederateAmbassadorFactory : public FederateAmbassadorFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DebugFederateAmbassadorFactory( const FederateAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver );
    virtual ~DebugFederateAmbassadorFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< plugins::hla::Federate_ABC > Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const;
    //@}

private:
    //! @name Member data
    //@{
    const FederateAmbassadorFactory_ABC& factory_;
    std::unique_ptr< ::hla::FederateAmbassador_ABC > ambassador_;
    //@}
};

}
}

#endif // plugins_hla_DebugFederateAmbassadorFactory_h
