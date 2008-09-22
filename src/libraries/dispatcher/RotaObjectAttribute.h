 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaObjectAttribute_h_
#define __RotaObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  RotaObjectAttribute
    @brief  RotaObjectAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class RotaObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RotaObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual ~RotaObjectAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributesSpecific& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const;
    //@}

private:
    unsigned int nDanger_;
    std::vector< unsigned int > nbcAgents_;
};

}

#endif // __RotaObjectAttribute_h_
