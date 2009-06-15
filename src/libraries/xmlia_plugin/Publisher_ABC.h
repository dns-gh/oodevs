// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
}

namespace plugins
{
namespace xmlia
{
    class ResponseHandler_ABC;

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher interface
*/
// Created: AGE 2008-05-30
// =============================================================================
class Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Publisher_ABC() {};
    virtual ~Publisher_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void PushReports() = 0;
    virtual xml::xostream& CreateReport() = 0;
    virtual void PullOrder( const std::string& message, ResponseHandler_ABC& handler ) = 0;
    //@}
};

}
}

#endif // __Publisher_ABC_h_
