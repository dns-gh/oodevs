 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DispersedMinedAreaObjectAttribute_h_
#define __DispersedMinedAreaObjectAttribute_h_

#include "game_asn/Asn.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

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
     DispersedMinedAreaObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg );
    ~DispersedMinedAreaObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_AttrObjectSpecific& asnMsg );
    virtual void Send     ( ASN1T_AttrObjectSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nMinesActivityTime_;
    double       rMinesDensity_;
};

}

#endif // __DispersedMinedAreaObjectAttribute_h_
