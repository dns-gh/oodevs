// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationRequest_h_
#define __DotationRequest_h_

namespace kernel
{
    class DotationType;
}

// =============================================================================
/** @class  DotationRequest
    @brief  DotationRequest
*/
// Created: AGE 2006-02-28
// =============================================================================
class DotationRequest
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationRequest( const kernel::DotationType& type, unsigned int requested, unsigned int granted, unsigned int convoyed );
    virtual ~DotationRequest();
    //@}

public:
    //! @name Member data
    //@{
    const kernel::DotationType* type_;
    unsigned int requested_;
    unsigned int granted_;
    unsigned int convoyed_;
    //@}
};

#endif // __DotationRequest_h_
