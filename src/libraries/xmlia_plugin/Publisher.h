// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_h_
#define __Publisher_h_

#include "Publisher_ABC.h"
#include <boost/thread/recursive_mutex.hpp>

namespace xml
{
    class xistream;
    class xostringstream;
    class xistringstream;
}


namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  Publisher
    @brief  Publisher
*/
// Created: SBO 2008-04-02
// =============================================================================
class Publisher : public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Publisher( xml::xistream& xis );
    virtual ~Publisher();
    //@}

    //! @name Operations
    //@{
    virtual void PushReports();
    virtual void PullOrder( const std::string& message, ResponseHandler_ABC& handler );
    virtual xml::xostream& CreateReport();
    virtual std::string GetUrlReports();
    virtual std::string GetXmliaMessage( const std::string& url );
    virtual std::string GetUrlId();
    virtual void PushReport( const std::string& xmliaMessage, const std::string& strPoe );
    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Publisher( const Publisher& );            //!< Copy constructor
    Publisher& operator=( const Publisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< xml::xostringstream > reports_;
    boost::recursive_mutex mutex_;
    std::string lastRequestTime_;
    //@}
};

}
}

#endif // __Publisher_h_
