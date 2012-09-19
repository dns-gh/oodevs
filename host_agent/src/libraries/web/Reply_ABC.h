// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef REPLY_ABC_H
#define REPLY_ABC_H

#include "HttpException.h"
#include <string>

namespace web
{
// =============================================================================
/** @class  Reply_ABC
    @brief  Web response interface
*/
// Created: BAX 2012-08-06
// =============================================================================
struct Reply_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
             Reply_ABC() {}
    virtual ~Reply_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetHeader( const std::string& key, const std::string& value ) = 0;
    virtual void SetStatus( HttpStatus code ) = 0;
    virtual void WriteHeaders() = 0;
    virtual int  WriteContent( const std::string& data ) = 0;
    virtual int  Write( const void* data, size_t size ) = 0;
    //@}
};
}

#endif // REPLY_ABC_H
