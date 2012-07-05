// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaObjectNameFactory_h
#define plugins_hla_HlaObjectNameFactory_h

#include "HlaObjectNameFactory_ABC.h"
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  HlaObjectNameFactory
    @brief  HlaObjectNameFactory
*/
// Created: AHC 2012-03-22
// =============================================================================
class HlaObjectNameFactory : public HlaObjectNameFactory_ABC
                           , private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    HlaObjectNameFactory( const std::string& federateName, const std::string& federateID );
    virtual ~HlaObjectNameFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::string CreateName( const std::string& hint ) const;
    //@}

private:
    const std::string federateName_;
    const std::string federateID_;
};

}
}
#endif // plugins_hla_HlaObjectNameFactory_h
