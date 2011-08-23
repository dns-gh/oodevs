// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequest_h_
#define __SupplyRequest_h_

#include "SupplyRequest_ABC.h"

namespace logistic {
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyRequestContainer
    @brief  SupplyRequestContainer
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequest : public SupplyRequest_ABC
{
public:
    //! @name Constructors/_
    //@{
             SupplyRequest( const PHY_DotationCategory& dotationCategory );
    virtual ~SupplyRequest();
    //@}

    //! @name Operations
    //@{
    virtual void   AddResource( boost::shared_ptr< SupplyResource_ABC > resource, double quantity );
    virtual bool   AffectSupplier( SupplySupplier_ABC& supplier );

    virtual double Supply          ();
    virtual double Convoy          ( double quantity ); // Return the quantity convoyed
    virtual double RemoveFromConvoy( double quantity ); // Return the quantity removed

    virtual void ReserveStock();
    virtual void ReturnStockNotAssignedToConvoy();
    //@}

    //! @name Accessors
    //@{
    virtual       SupplySupplier_ABC*   GetSupplier        () const;
    virtual const PHY_DotationCategory& GetDotationCategory() const;
    virtual       double                GetGrantedValue    () const;
    virtual       bool                  IsComplementary    () const;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::SupplyResourceRequest& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< boost::shared_ptr< SupplyResource_ABC > , double /*quantity requested*/ > > T_ResourceRequests;
    //@}

private:
    const PHY_DotationCategory& dotationCategory_;
    T_ResourceRequests resourceRequests_;
    SupplySupplier_ABC* supplier_;
    double requestedQuantity_;
    double grantedQuantity_;
    double convoyedQuantity_;
    bool complementarySupply_;
};

} // end namespace logistic

#endif // __SupplyRequest_ABC_h_
