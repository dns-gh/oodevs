// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestParameters_ABC_h_
#define __SupplyRequestParameters_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class PHY_ComposanteTypePion;

namespace sword
{
    class Pathfind;
}

namespace logistic {
    class SupplyConvoyFactory_ABC;

// =============================================================================
/** @class  SupplyRequestParameters_ABC
    @brief  SupplyRequestParameters_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestParameters_ABC : private boost::noncopyable
{
public:
    //! @name Types to ease the pain ...
    //@{
    typedef const std::vector< std::pair< const PHY_ComposanteTypePion* , unsigned > > T_Transporters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequestParameters_ABC() {}
    virtual ~SupplyRequestParameters_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual       SupplySupplier_ABC*                             GetTransportersProvider      () const = 0;
    virtual const T_Transporters&                                 GetTransporters              () const = 0;
    virtual const SupplyConvoyFactory_ABC&                        GetConvoyFactory             () const = 0;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > > GetPathToTransportersProvider() const = 0;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > > GetPathToSupplier            () const = 0;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > > GetPathToRecipient( SupplyRecipient_ABC& recipient ) const = 0;

    virtual void ToRecipientItinerary( SupplyRecipient_ABC& recipient, sword::Pathfind& pathfind ) const = 0;
    virtual void ToSupplierItinerary( sword::Pathfind& pathfind ) const = 0;
    virtual void ToTransportersItinerary( sword::Pathfind& pathfind ) const = 0;

    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace logistic

#endif // __SupplyRequestParameters_ABC_h_
