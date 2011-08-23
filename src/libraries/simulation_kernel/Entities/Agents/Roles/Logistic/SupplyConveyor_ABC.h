// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConveyor_ABC_h_
#define __SupplyConveyor_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class PHY_DotationCategory;
class MIL_AgentPion;

namespace logistic {
    class SupplyConvoyEventsObserver_ABC;

// =============================================================================
/** @class  SupplyConveyor_ABC
    @brief  SupplyConveyor_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConveyor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConveyor_ABC() {}
    virtual ~SupplyConveyor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double   Convoy               ( SupplyConvoyEventsObserver_ABC& eventsObserver, const PHY_DotationCategory& dotationCategory, double quantity ) = 0;
    virtual bool     IsFull               () const = 0;
    virtual void     LendTo               ( MIL_AgentPion& convoyPion ) = 0;
    virtual unsigned ApproximateTravelTime( const MT_Vector2D& vStartPos, const MT_Vector2D& vEndPos ) const = 0;
    virtual double   Supply               ( const PHY_DotationCategory& dotationCategory, double quantity ) = 0;
    virtual void     Destroy              ( SupplyConvoyEventsObserver_ABC& eventsObserver ) = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyConveyor_ABC_h_
