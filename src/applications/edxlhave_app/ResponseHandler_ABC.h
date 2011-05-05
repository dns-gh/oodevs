// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __edxl_ResponseHandler_ABC_h_
#define __edxl_ResponseHandler_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xisubstream;
}

namespace edxl
{
// =============================================================================
/** @class  ResponseHandler_ABC
    @brief  ResponseHandler_ABC
*/
// Created: SBO 2008-05-22
// =============================================================================
class ResponseHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ResponseHandler_ABC() {}
    virtual ~ResponseHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Handle( xml::xisubstream response ) = 0;
    //@}
};

}

#endif // __ResponseHandler_ABC_h_
