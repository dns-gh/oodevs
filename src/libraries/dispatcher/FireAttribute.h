 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class FireAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     FireAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    int          heat_;
    unsigned int nFireClass_; // XML reference - no resolved by dispatcher
    //@}
};

}

#endif // __FireAttribute_h_
