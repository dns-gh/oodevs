 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineAttribute_h_
#define __MineAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class MineAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MineAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~MineAttribute();
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
    unsigned     dotation_;    
    unsigned int nNbrDotationForMining_;
    unsigned int nPercentageMining_;
    //@}
};

}

#endif // __MineAttribute_h_
