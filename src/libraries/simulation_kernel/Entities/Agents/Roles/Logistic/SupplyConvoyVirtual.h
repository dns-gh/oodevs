// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyVirtual_h_
#define __SupplyConvoyVirtual_h_

#include "SupplyConvoy.h"
#include "MT_Tools/MT_Vector2DTypes.h"

namespace logistic {

// =============================================================================
/** @class  SupplyConvoyVirtual
    @brief  SupplyConvoyVirtual
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoyVirtual : public SupplyConvoy
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoyVirtual( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters );
    virtual ~SupplyConvoyVirtual();
    //@}

    //! @name Operations
    //@{
    virtual unsigned MoveToSupplier            ();
    virtual unsigned MoveToSupplyRecipient     ();
    virtual unsigned MoveToTransportersProvider();
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::UnitId& msg ) const;
    //@}

private:
    //! @name Tools
    //@{
    unsigned ComputeTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const;
    unsigned MoveTo           ( const MT_Vector2D& position );
    //@}

private:
    MT_Vector2D position_;
};

} // end namespace logistic

#endif // __SupplyConvoyVirtual_h_
