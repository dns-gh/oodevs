// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __edxl_Publisher_ABC_h_
#define __edxl_Publisher_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace edxl
{
    class ResponseHandler_ABC;

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher interface
*/
// Created: JCR 2010-05-29
// =============================================================================
class Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Publisher_ABC() {}
    virtual ~Publisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void PullSituation( const std::string& message, ResponseHandler_ABC& response ) = 0;
    virtual void PushReport( const std::string& message ) = 0;
    //@}
};

}

#endif // __Publisher_ABC_h_
