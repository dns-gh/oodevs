// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyReal_ABC_h_
#define __SupplyConvoyReal_ABC_h_

#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/export.hpp>

class PHY_ComposantePion;

namespace moving
{
    class SpeedComputer_ABC;
}

namespace logistic {
    class SupplyRecipient_ABC;
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyConvoyReal_ABC
    @brief  SupplyConvoyReal_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyReal_ABC : public boost::enable_shared_from_this< SupplyConvoyReal_ABC >
                           , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyReal_ABC() {}
    virtual ~SupplyConvoyReal_ABC() {}
    //@}

    //! @name Events
    //@{
    virtual void NotifyMovedToSupplier            () = 0;
    virtual void NotifyMovedToTransportersProvider() = 0;
    virtual void NotifyMovedToSupplyRecipient     () = 0;
    virtual void NotifyTransporterDestroyed       ( PHY_ComposantePion& transporter ) = 0;
    virtual void NotifyConvoyEndMission           () = 0;
    //@}

    //! @name Accessors
    //@{
    virtual int                                             GetCurrentAction         () const = 0;
    virtual SupplyRecipient_ABC*                            GetCurrentSupplyRecipient() const = 0;
    virtual SupplySupplier_ABC&                             GetSupplier              () const = 0;
    virtual SupplySupplier_ABC&                             GetTransportersProvider  () const = 0;
    virtual std::vector< boost::shared_ptr< MT_Vector2D > > GetPathToNextDestination () const = 0;
    virtual const MIL_Agent_ABC*                            GetProvider              () const = 0;
    virtual bool                                            IsPushedFlow             () const = 0;
    virtual void                                            ToItinerary( sword::Pathfind& pathfind ) const = 0;
    //@}
    
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
};

} // end namespace logistic

#endif // __SupplyConvoyReal_ABC_h_
