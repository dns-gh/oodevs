// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __timeline_ResponseHandler_ABC_h_
#define __timeline_ResponseHandler_ABC_h_

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace timeline
{
// =============================================================================
/** @class  ResponseHandler_ABC
    @brief  ResponseHandler_ABC
*/
// Created: SBO 2008-05-22
// =============================================================================
class ResponseHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResponseHandler_ABC() {}
    virtual ~ResponseHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Handle( xml::xistream& response ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResponseHandler_ABC( const ResponseHandler_ABC& );            //!< Copy constructor
    ResponseHandler_ABC& operator=( const ResponseHandler_ABC& ); //!< Assignment operator
    //@}
};

}
}

#endif // __timeline_ResponseHandler_ABC_h_
