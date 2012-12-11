// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __geostore_DatabaseException_h_
#define __geostore_DatabaseException_h_

#include <tools/Exception.h>

namespace geostore
{

// =============================================================================
/** @class  NET_AsnException
    @brief  NET_AsnException
*/
// Created: NLD 2006-11-13
// =============================================================================
class DatabaseException : public tools::Exception
{
public:
    //! @name Constructors/destructor
    //@{
             DatabaseException( int errCode, const std::string& file, const std::string& function, const unsigned int line, const std::string& what ) throw()
                 : tools::Exception( file, function, line, what )
                 , err_              ( errCode )
             {}
    virtual ~DatabaseException() throw() {}
    //@}

    //! @name Accessors
    //@{
    int GetErrorCode() const throw() { return err_; }
    //@}

private:
    //! @name Member data
    //@{
    const int err_;
    //@}
};

} //! namespace geostore

#define MASA_EXCEPTION_SQLITE( errorCode, errorMsg ) geostore::DatabaseException( errorCode, __FILE__, __FUNCTION__, __LINE__, errorMsg )

#endif // __geostore_DatabaseException_h_
