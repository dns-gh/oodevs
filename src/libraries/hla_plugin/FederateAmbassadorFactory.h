// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FederateAmbassadorFactory_h
#define plugins_hla_FederateAmbassadorFactory_h

#include "FederateAmbassadorFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  FederateAmbassadorFactory
    @brief  Federate ambassador factory
*/
// Created: SLI 2011-05-27
// =============================================================================
class FederateAmbassadorFactory : public FederateAmbassadorFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FederateAmbassadorFactory( unsigned int lookAhead );
    virtual ~FederateAmbassadorFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< plugins::hla::Federate_ABC > Create( ::hla::RtiAmbassador_ABC& ambassador, const std::string& name, int lookAhead ) const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int lookAhead_;
    //@}
};

}
}

#endif // plugins_hla_FederateAmbassadorFactory_h
