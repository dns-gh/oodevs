// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequest.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:20 $
// $Revision: 2 $
// $Workfile: PHY_SupplyDotationRequest.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationRequest_h_
#define __PHY_SupplyDotationRequest_h_

#include "PHY_SupplyRequest_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"

// =============================================================================
// @class  PHY_SupplyDotationRequest
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationRequest : public PHY_SupplyRequest_ABC< PHY_Dotation >
{
public:
             PHY_SupplyDotationRequest();
    virtual ~PHY_SupplyDotationRequest();

    //! @name 
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void AddDotation( PHY_Dotation& dotation );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_SupplyDotationRequest::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyDotationRequest::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_SupplyRequest_ABC< PHY_Dotation > >( *this );
}

#endif // __PHY_SupplyDotationRequest_h_
