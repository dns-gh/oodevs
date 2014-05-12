// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FederateFactory_h
#define plugins_hla_FederateFactory_h

#include <memory>

namespace hla
{
    class RtiAmbassador_ABC;
}

namespace xml
{
    class xisubstream;
}

namespace tools
{
    class Path;
}

namespace plugins
{
namespace hla
{

class FederateAmbassadorFactory_ABC;
class Federate_ABC;

// =============================================================================
/** @class  FederateFactory
    @brief  FederateFactory
*/
// Created: AHC 2012-11-09
// =============================================================================
class FederateFactory
{
public:
    //! @name Operations
    //@{
    static std::unique_ptr< Federate_ABC > CreateFederate( xml::xisubstream xis, ::hla::RtiAmbassador_ABC& ambassador,
            const FederateAmbassadorFactory_ABC& factory, const tools::Path& pluginDirectory );
    //@}

};

}
}

#endif // plugins_hla_FederateFactory_h
