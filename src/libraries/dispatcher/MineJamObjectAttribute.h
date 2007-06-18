 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineJamObjectAttribute_h_
#define __MineJamObjectAttribute_h_

#include "game_asn/Asn.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  MineJamObjectAttribute
    @brief  MineJamObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class MineJamObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MineJamObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg );
    ~MineJamObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nMinesActivityTime_;
};

}

#endif // __MineJamObjectAttribute_h_
