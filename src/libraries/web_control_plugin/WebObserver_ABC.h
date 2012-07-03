// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WEB_SERVER_ABC_H
#define WEB_SERVER_ABC_H

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace web_control
{
// =============================================================================
/** @class  WebObserver_ABC
    @brief  Web observer definition
*/
// Created: BAX 2012-02-28
// =============================================================================
class WebObserver_ABC : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
             WebObserver_ABC() {}
    virtual ~WebObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Notify( const std::string& method, const std::string& uri ) = 0;
    //@}
};

}
}

#endif // WEB_SERVER_ABC_H
