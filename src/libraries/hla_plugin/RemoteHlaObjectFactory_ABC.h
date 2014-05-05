// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteHlaObjectFactory_ABC_h
#define plugins_hla_RemoteHlaObjectFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>

namespace plugins
{
namespace hla
{
    class HlaObject_ABC;
    class ObjectListener_ABC;

// =============================================================================
/** @class  RemoteHlaObjectFactory_ABC
    @brief  Remote hla object factory definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteHlaObjectFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteHlaObjectFactory_ABC() {}
    virtual ~RemoteHlaObjectFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< HlaObject_ABC > Create( const std::string& name ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_RemoteHlaObjectFactory_ABC_h
