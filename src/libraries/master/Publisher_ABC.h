 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

struct ASN1T_MsgsOutMaster;

namespace master
{
// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher_ABC
*/
// Created: NLD 2007-01-30
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
    virtual void Send( const ASN1T_MsgsOutMaster& msg ) = 0;
    //@}
};

}

#endif // __Publisher_ABC_h_
