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

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher interface
*/
// Created: SLG 2009-06-12
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
    virtual std::string GetUrlReports() = 0;
    virtual std::string GetXmliaMessage( const std::string& url ) = 0;
    virtual std::string GetUrlId() = 0;
    virtual void PushReport( const std::string& xmliaMessage, const std::string& strPoe ) = 0;
    //@}
};

}
}

#endif // __Publisher_ABC_h_
