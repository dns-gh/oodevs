 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DispersedMinedAreaObjectAttribute_h_
#define __DispersedMinedAreaObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  DispersedMinedAreaObjectAttribute
    @brief  DispersedMinedAreaObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class DispersedMinedAreaObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DispersedMinedAreaObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual ~DispersedMinedAreaObjectAttribute();
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

#endif // __DispersedMinedAreaObjectAttribute_h_
