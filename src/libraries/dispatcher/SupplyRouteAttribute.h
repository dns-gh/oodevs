 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class SupplyRouteAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     SupplyRouteAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~SupplyRouteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    bool         bEquipped_;
    unsigned int nMaxWeight_;
    unsigned int nWidth_;
    unsigned int nLength_;
    unsigned int nFlow_;
};

}

#endif // __SupplyRouteAttribute_h_
