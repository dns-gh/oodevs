// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RtiAmbassadorFactory_ABC_h
#define plugins_hla_RtiAmbassadorFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <hla/RtiAmbassador_ABC.h>

namespace hla
{
    class TimeFactory_ABC;
    class TimeIntervalFactory_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RtiAmbassadorFactory_ABC
    @brief  Rti ambassador factory definition
*/
// Created: SLI 2011-01-10
// =============================================================================
class RtiAmbassadorFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RtiAmbassadorFactory_ABC() {}
    virtual ~RtiAmbassadorFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual ::hla::RtiAmbassador_ABC* CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                        ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                        const std::string& lrcSettings ) const = 0;
    virtual void DeleteAmbassador( ::hla::RtiAmbassador_ABC* ambassador ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_RtiAmbassadorFactory_ABC_h
