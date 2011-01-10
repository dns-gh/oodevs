// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RtiAmbassadorFactory_h
#define plugins_hla_RtiAmbassadorFactory_h

#include "RtiAmbassadorFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RtiAmbassadorFactory
    @brief  Rti ambassador factory
*/
// Created: SLI 2011-01-10
// =============================================================================
class RtiAmbassadorFactory : public RtiAmbassadorFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RtiAmbassadorFactory();
    virtual ~RtiAmbassadorFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< ::hla::RtiAmbassador_ABC > CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                        ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                        const std::string& host, const std::string& port ) const;
    //@}
};

}
}

#endif // plugins_hla_RtiAmbassadorFactory_h
