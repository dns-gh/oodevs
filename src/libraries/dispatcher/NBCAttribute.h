 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttribute_h_
#define __NBCAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  NBCAttribute
    @brief  NBCAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class NBCAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     NBCAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~NBCAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}
private:
    //! @name 
    //@{
    void Clear();
    //@}
private:
    ASN1T_ObjectAttributeNBC nbc_; // XML reference - no resolved by dispatcher
};

}

#endif // __NBCAttribute_h_
