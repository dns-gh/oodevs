// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NET_AsnException_h_
#define __NET_AsnException_h_

#include <tools/Exception.h>

// =============================================================================
/** @class  NET_AsnException
    @brief  NET_AsnException
*/
// Created: NLD 2006-11-13
// =============================================================================
template< typename T >
class NET_AsnException : public tools::Exception
{
public:
    //! @name Constructors/Destructor
    //@{
             NET_AsnException( T nErrorID, const std::string& file, const std::string& function, const unsigned int line, const std::string& what ) throw()
                 : tools::Exception( file, function, line, what )
                 , nErrorID_( nErrorID )
             {}
    virtual ~NET_AsnException() throw() {}
    //@}

    //! @name Operations
    //@{
    T GetErrorID() const throw() { return nErrorID_; }
    //@}

private:
    //! @name Member data
    //@{
    T nErrorID_;
    //@}
};

#define MASA_EXCEPTION_ASN( ErrorIdType, ErrorId ) NET_AsnException< ErrorIdType >( ErrorId, __FILE__, __FUNCTION__, __LINE__, "invalid message" )

template< typename T>
class NET_AsnBadParam : public NET_AsnException< T >
{
public:
             NET_AsnBadParam( T nErrorID, const std::string& file,
                     const std::string& function, const unsigned int line,
                     const std::string& what ) throw()
                 : NET_AsnException( nErrorID, file, function, line, what )
             {}
    virtual ~NET_AsnBadParam() throw() {}
};

#define MASA_BADPARAM_ASN( ErrorIdType, ErrorId, Name ) NET_AsnBadParam< ErrorIdType >( ErrorId, __FILE__, __FUNCTION__, __LINE__, Name )

class NET_InvalidTasker : public tools::Exception
{
public:
    NET_InvalidTasker( const std::string& file, const std::string& function,
            const unsigned int line ) throw()
        : tools::Exception( file, function, line, "invalid tasker" )
    {}
    virtual ~NET_InvalidTasker() throw() {}
};

#define MASA_INVALIDTASKER NET_InvalidTasker( __FILE__, __FUNCTION__, __LINE__ )

class NET_ConnectionClosed: public tools::Exception
{
public:
    NET_ConnectionClosed( const std::string& file, const std::string& function,
            const unsigned int line, const std::string& what ) throw()
        : tools::Exception( file, function, line, what )
    {}
};

#define MASA_CONNECTIONCLOSED( what ) NET_ConnectionClosed( __FILE__, __FUNCTION__, __LINE__, what )

#endif // __NET_AsnException_h_
