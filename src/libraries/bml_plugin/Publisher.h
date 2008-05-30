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

namespace xml
{
    class xistream;
    class xostringstream;
}

class DCSOperationsSoapBindingProxy;

namespace bml
{
    class ResponseHandler_ABC;

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
	void PushReports();
    xml::xostream& CreateReport();
    void PullOrder( const std::string& message, ResponseHandler_ABC& handler );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Publisher( const Publisher& );            //!< Copy constructor
    Publisher& operator=( const Publisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ConfigureService( DCSOperationsSoapBindingProxy& service ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string endpoint_;
    const std::string proxyHost_;
    const unsigned short proxyPort_;
    std::auto_ptr< xml::xostringstream > reports_;
    //@}
};

}

#endif // __Publisher_h_
