// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_DebugRtiAmbassadorFactory_h
#define plugins_hla_DebugRtiAmbassadorFactory_h

#include "RtiAmbassadorFactory_ABC.h"

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{
    class ObjectResolver_ABC;

// =============================================================================
/** @class  DebugRtiAmbassadorFactory
    @brief  DebugRtiAmbassadorFactory
*/
// Created: SLI 2011-05-27
// =============================================================================
class DebugRtiAmbassadorFactory : public RtiAmbassadorFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DebugRtiAmbassadorFactory( const RtiAmbassadorFactory_ABC& factory, dispatcher::Logger_ABC& logger, ObjectResolver_ABC& resolver );
    virtual ~DebugRtiAmbassadorFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< ::hla::RtiAmbassador_ABC > CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                        ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                        const std::string& host, const std::string& port ) const;
    //@}

private:
    //! @name Member data
    //@{
    const RtiAmbassadorFactory_ABC& factory_;
    dispatcher::Logger_ABC& logger_;
    ObjectResolver_ABC& resolver_;
    //@}
};

}
}

#endif // plugins_hla_DebugRtiAmbassadorFactory_h
