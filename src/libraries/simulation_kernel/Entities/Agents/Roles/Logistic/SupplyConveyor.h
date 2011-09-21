// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConveyor_h_
#define __SupplyConveyor_h_

#include "SupplyConveyor_ABC.h"

class PHY_ComposantePion;
class MIL_AgentPion;

namespace logistic {

// =============================================================================
/** @class  SupplyConveyor
    @brief  SupplyConveyor
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConveyor : public SupplyConveyor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConveyor( PHY_ComposantePion& composante, MIL_AgentPion& pion );
    virtual ~SupplyConveyor();
    //@}

    //! @name Operations
    //@{
    virtual double   Convoy               ( SupplyConvoyEventsObserver_ABC& eventsObserver, const PHY_DotationCategory& dotationCategory, double quantity );
    virtual bool     IsFull               () const;
    virtual bool     IsEmpty              () const;
    virtual void     LendTo               ( MIL_AgentPion& convoyPion );
    virtual unsigned ApproximateTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const;
    virtual double   Supply               ( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void     Destroy              ( SupplyConvoyEventsObserver_ABC& eventsObserver );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_Resources;
    //@}

private:
    //! @name Tools
    //@{
    void UndoLend();
    //@}

private:
    PHY_ComposantePion& composante_;
    MIL_AgentPion&      pion_;
    double              weightCapacity_;
    double              volumeCapacity_;
    T_Resources         resourcesConvoyed_;
    MIL_AgentPion*      borrower_;
};

} // end namespace logistic

#endif // __SupplyConveyor_h_
