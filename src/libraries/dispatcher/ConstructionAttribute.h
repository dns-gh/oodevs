 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ConstructionAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ConstructionAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~ConstructionAttribute();
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
    unsigned int nPercentageConstruction_;
    unsigned int nNbrDotationForConstruction_;
    //@}
    
};

}

#endif // __ConstructionAttribute_h_
