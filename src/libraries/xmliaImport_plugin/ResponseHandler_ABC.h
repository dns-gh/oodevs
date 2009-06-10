// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ResponseHandler_ABC_h_
#define __ResponseHandler_ABC_h_

namespace plugins
{
namespace xmliaImport
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
    virtual void Handle( const std::string& response ) = 0;
    //@}
};

}
}

#endif // __ResponseHandler_ABC_h_
