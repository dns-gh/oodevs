// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsignData_ABC_h_
#define __ConsignData_ABC_h_

#include <sstream>
#include <boost/noncopyable.hpp>
#include "tools/FileWrapper.h"

namespace plugins
{
namespace logistic
{
    class ConsignWriter;

// =============================================================================
/** @class  ConsignData_ABC
    @brief  ConsignData_ABC
*/
// Created: MMC 2012-08-06
// =============================================================================
class ConsignData_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
            ConsignData_ABC( const std::string& requestId ) : requestId_( requestId ) {}
    virtual ~ConsignData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void WriteConsign( ConsignWriter& w ) const = 0;
    //@}

public:
    //! @name Member data
    //@{
    std::string tick_;
    std::string simTime_;
    //@}

protected:
    //! @name Member data
    //@{
    std::string requestId_;
    //@}
};

}
}

#endif // __ConsignData_ABC_h_
