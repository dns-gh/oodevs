 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteObjectAttribute_h_
#define __CrossingSiteObjectAttribute_h_

#include "game_asn/Asn.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  CrossingSiteObjectAttribute
    @brief  CrossingSiteObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class CrossingSiteObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     CrossingSiteObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg );
    ~CrossingSiteObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nWidth_;
    unsigned int nDepth_;
    unsigned int nSpeed_;
    bool         bNeedsConstruction_;    
};

}

#endif // __CrossingSiteObjectAttribute_h_
