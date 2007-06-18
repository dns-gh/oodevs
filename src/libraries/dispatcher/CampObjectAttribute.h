 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CampObjectAttribute_h_
#define __CampObjectAttribute_h_

#include "game_asn/Asn.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;
    class Automat;

// =============================================================================
/** @class  CampObjectAttribute
    @brief  CampObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
    class CampObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     CampObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg );
    ~CampObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    const Model&   model_;
    const Automat* pTC2_;
};

}

#endif // __CampObjectAttribute_h_
