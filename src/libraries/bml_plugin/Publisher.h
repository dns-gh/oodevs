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

namespace xml
{
    class xistream;
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
class Publisher
{

public:
    //! @name Constructors/Destructor
    //@{
             Publisher( xml::xistream& xis );
    virtual ~Publisher();
    //@}

    //! @name Operations
    //@{
    void PushReport( const std::string& message );
    void PullOrder ( const std::string& message, ResponseHandler_ABC& handler );
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
    //@}
};

}

#endif // __Publisher_h_
