// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestContainer_ABC_h_
#define __SupplyRequestContainer_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class PHY_DotationCategory;
class PHY_ComposanteTypePion;

namespace logistic {
    class SupplyResource_ABC;
    class SupplyRequestDispatcher_ABC;
    class SupplyRecipient_ABC;
    class SupplySupplier_ABC;
    class SupplyConvoyFactory_ABC;

// =============================================================================
/** @class  SupplyRequestContainer_ABC
    @brief  SupplyRequestContainer_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestContainer_ABC : private boost::noncopyable
{
public:
    //! @name Types to ease the pain ...
    //@{
    typedef const std::vector< std::pair< const PHY_ComposanteTypePion* , unsigned > > T_Transporters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequestContainer_ABC() {}
    virtual ~SupplyRequestContainer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetTransportersProvider      ( SupplySupplier_ABC* provider ) = 0;
    virtual void SetTransporters              ( const T_Transporters& transporters ) = 0;
    virtual void SetPathToRecipient           ( SupplyRecipient_ABC& recipient, const T_PointVector& wayPoints ) = 0;
    virtual void SetPathToTransportersProvider( const T_PointVector& wayPoints ) = 0;
    virtual void SetPathToSupplier            ( const T_PointVector& wayPoints ) = 0;
    virtual void SetConvoyFactory             ( const SupplyConvoyFactory_ABC& convoyFactory ) = 0;
    virtual void AddResource                  ( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyResource_ABC > resource, double quantity ) = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyRequestContainer_ABC_h_
