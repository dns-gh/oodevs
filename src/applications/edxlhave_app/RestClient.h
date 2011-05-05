// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RestClient_h_
#define __RestClient_h_

#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif
#pragma warning( push, 0 )
#include <boost/asio.hpp>
#pragma warning( pop )
#include <string>

#include "Connection.h"

namespace edxl
{

// =============================================================================
/** @class  RestClient
    @brief  RestClient
*/
// Created: JCR 2010-05-29
// =============================================================================
class RestClient
{

public:
    //! @name Constructors/Destructor
    //@{
             RestClient( const std::string& host, const std::string& path , bool useSsl);
    virtual ~RestClient();
    //@}

    //! @name Operators
    //@{
    void DoPost( const std::string& message, std::string& response );
    void DoGet( const std::string& message, std::string& response );
    //@}

    //! @name Accessors
    //@{
    unsigned GetStatus() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RestClient( const RestClient& );            //!< Copy constructor
    RestClient& operator=( const RestClient& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ProceedRequest( boost::asio::streambuf& response, std::string& contentß );
    void ReadHeader( boost::asio::streambuf& response );
    void ReadContent( boost::asio::streambuf& response, std::string& content );
    //@}

private:
    //! @name Network Member data
    //@{
    std::string host_;
    std::string path_;
    bool useChunk_;
    Connection socket_;
    //@}

    //! @name Member data
    //@{
    unsigned status_;
    //@}
};

}

#endif // __RestClient_h_
