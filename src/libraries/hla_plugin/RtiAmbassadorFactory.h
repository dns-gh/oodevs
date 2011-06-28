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

namespace xml
{
    class xisubstream;
}

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
             RtiAmbassadorFactory( xml::xisubstream configuration, xml::xisubstream protocols );
    virtual ~RtiAmbassadorFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< ::hla::RtiAmbassador_ABC > CreateAmbassador( ::hla::TimeFactory_ABC& timeFactory, ::hla::TimeIntervalFactory_ABC& timeIntervalFactory,
                                                                        ::hla::RtiAmbassador_ABC::E_MessagePolicy policy,
                                                                        const std::string& host, const std::string& port ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef ::hla::RtiAmbassador_ABC* (*T_CreateAmbassador)( ::hla::TimeFactory_ABC&, ::hla::TimeIntervalFactory_ABC&, ::hla::RtiAmbassador_ABC::E_MessagePolicy, const std::string&, const std::string& );
    //@}

private:
    //! @name Member data
    //@{
    T_CreateAmbassador createAmbassador;
    //@}
};

}
}

#endif // plugins_hla_RtiAmbassadorFactory_h
