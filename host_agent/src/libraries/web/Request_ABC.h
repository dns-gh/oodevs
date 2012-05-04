// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef REQUEST_ABC_H
#define REQUEST_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>
#include <string>
#include <vector>

namespace web
{
// =============================================================================
/** @class  Request_ABC
    @brief  Request definition
*/
// Created: BAX 2012-02-28
// =============================================================================
class Request_ABC : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
             Request_ABC() {}
    virtual ~Request_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetMethod() const = 0;
    virtual std::string GetUri() const = 0;
    virtual boost::optional< std::string > GetParameter( const std::string& name ) const = 0;
    virtual boost::optional< std::string > GetHeader( const std::string& name ) const = 0;
    //@}

    //! @name Mime methods
    //@{
    typedef std::vector< std::string > T_Names;
    typedef std::vector< boost::reference_wrapper< std::istream > > T_Streams;
    virtual T_Streams ReadMimeParts( const T_Names& names ) = 0;
    //@}
};

}

#endif // REQUEST_ABC_H
