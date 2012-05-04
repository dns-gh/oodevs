// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SECURE_POOL_H
#define SECURE_POOL_H

#include "Pool_ABC.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
// =============================================================================
/** @class  SecurePool
    @brief  Secure thread pool class definition
*/
// Created: BAX 2012-04-16
// =============================================================================
class SecurePool : public boost::noncopyable
{
public:
    //! @name Constructors/Destructors
    //@{
             SecurePool( cpplog::BaseLogger& log, const std::string& name, Pool_ABC& pool );
    virtual ~SecurePool();
    //@}

    //! @name Methods
    //@{
    virtual void Post( const Pool_ABC::Task& task );
    virtual Pool_ABC& Get() const;
    //@}

private:
    //! @name Private members
    //@{
    cpplog::BaseLogger& log_;
    const std::string name_;
    const std::auto_ptr< boost::mutex > access_;
    std::vector< Pool_ABC::Future > futures_;
    Pool_ABC& pool_;
    //@}
};

}

#endif // SECURE_POOL_H
