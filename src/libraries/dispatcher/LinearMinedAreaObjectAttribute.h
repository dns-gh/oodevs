 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinearMinedAreaObjectAttribute_h_
#define __LinearMinedAreaObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  LinearMinedAreaObjectAttribute
    @brief  LinearMinedAreaObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class LinearMinedAreaObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LinearMinedAreaObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual ~LinearMinedAreaObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nMinesActivityTime_;
    double       rMinesDensity_;
};

}

#endif // __LinearMinedAreaObjectAttribute_h_
