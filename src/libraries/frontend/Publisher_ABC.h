// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

struct ASN1T_MsgsInMaster;

namespace frontend
{

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher_ABC
*/
// Created: SBO 2007-01-25
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
    virtual void Send( const ASN1T_MsgsInMaster& message ) = 0;
    //@}
};

}

#endif // __Publisher_ABC_h_
