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
#include <boost/serialization/export.hpp>
#include <tools/Map.h>

class PHY_ComposantePion;
class MIL_AgentPion;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace logistic
{
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
             SupplyConveyor();
             SupplyConveyor( PHY_ComposantePion& composante, MIL_AgentPion& pion );
    virtual ~SupplyConveyor();
    //@}

    //! @name Operations
    //@{
    virtual double   Convoy               ( SupplyConvoyEventsObserver_ABC& eventsObserver, const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void     LendTo               ( MIL_AgentPion& convoyPion );
    virtual unsigned ApproximateTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const;
    virtual double   Supply               ( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void     Destroy              ( SupplyConvoyEventsObserver_ABC& eventsObserver );

    virtual bool     CanTransport         ( const PHY_DotationCategory& dotationCategory ) const;
    void serialize( MIL_CheckPointInArchive&, const unsigned int );
    void serialize( MIL_CheckPointOutArchive&, const unsigned int );
    //@}

private:
    //! @name Tools
    //@{
    void UndoLend();
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, double > T_Resources;
    //@}

private:
    PHY_ComposantePion* composante_;
    MIL_AgentPion*      pion_;
    double              weightCapacity_;
    double              volumeCapacity_;
    T_Resources         resourcesConvoyed_;
    MIL_AgentPion*      borrower_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyConveyor );

#endif // __SupplyConveyor_h_
