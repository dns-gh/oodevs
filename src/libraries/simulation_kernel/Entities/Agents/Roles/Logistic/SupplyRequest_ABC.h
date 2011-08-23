// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequest_ABC_h_
#define __SupplyRequest_ABC_h_

class PHY_DotationCategory;

namespace sword {
    class SupplyResourceRequest;
}

namespace logistic {
    class SupplyResource_ABC;
    class SupplySupplier_ABC;
    class LogisticLink_ABC;

// =============================================================================
/** @class  SupplyRequest_ABC
    @brief  SupplyRequest_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequest_ABC() {}
    virtual ~SupplyRequest_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void   AddResource( boost::shared_ptr< SupplyResource_ABC > resource, double quantity ) = 0;
    virtual bool   AffectSupplier( SupplySupplier_ABC& supplier ) = 0;
    virtual bool   AffectSupplier( boost::shared_ptr< LogisticLink_ABC > supplier ) = 0;
    
    virtual double Supply          () = 0; // Return the quantity supplied;
    virtual double Convoy          ( double quantity ) = 0; // Return the quantity convoyed
    virtual double RemoveFromConvoy( double quantity ) = 0; // Return the quantity removed

    virtual void ReserveStock() = 0;
    virtual void ReturnStockNotAssignedToConvoy() = 0;

    virtual bool IsComplementary() const = 0;
    //@}
    
    //! @name Accessors
    //@{
    virtual       SupplySupplier_ABC*   GetSupplier        () const = 0;
    virtual const PHY_DotationCategory& GetDotationCategory() const = 0;
    virtual       double                GetGrantedValue    () const = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::SupplyResourceRequest& msg ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyRequest_ABC_h_
