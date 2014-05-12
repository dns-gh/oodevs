// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FederateAmbassadorFactory_ABC_h
#define plugins_hla_FederateAmbassadorFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

namespace hla
{
    class RtiAmbassador_ABC;
}

namespace plugins
{
namespace hla
{
    class Federate_ABC;

// =============================================================================
/** @class  FederateAmbassadorFactory_ABC
    @brief  Federate ambassador factory defintion
*/
// Created: SLI 2011-05-27
// =============================================================================
class FederateAmbassadorFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateAmbassadorFactory_ABC() {}
    virtual ~FederateAmbassadorFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< plugins::hla::Federate_ABC > Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_FederateAmbassadorFactory_ABC_h
