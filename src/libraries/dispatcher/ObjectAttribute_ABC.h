 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttribute_ABC_h_
#define __ObjectAttribute_ABC_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ObjectAttribute_ABC
    @brief  ObjectAttribute_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ObjectAttribute_ABC( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg );
    ~ObjectAttribute_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg ) = 0;
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const = 0;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const = 0;
    //@}

protected:
    unsigned int nType_;
};

}

#endif // __ObjectAttribute_ABC_h_
