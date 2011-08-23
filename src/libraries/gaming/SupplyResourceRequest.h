// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyResourceRequest_h_
#define __SupplyResourceRequest_h_

namespace kernel
{
    class DotationType;
}

// =============================================================================
/** @class  SupplyResourceRequest
    @brief  SupplyResourceRequest
*/
// Created: AGE 2006-02-28
// =============================================================================
class SupplyResourceRequest
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResourceRequest( const kernel::DotationType& type, unsigned int requested, unsigned int granted, unsigned int convoyed );
    virtual ~SupplyResourceRequest();
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
