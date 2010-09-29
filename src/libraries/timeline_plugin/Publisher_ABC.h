// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __timeline_Publisher_ABC_h_
#define __timeline_Publisher_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace timeline
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
        virtual void PullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& response ) = 0;
        virtual void PushReport( const std::string& message ) = 0;
        //@}
};

}
}

#endif // __Publisher_ABC_h_
