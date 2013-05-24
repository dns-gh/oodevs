// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinePositions_h_
#define __TacticalLinePositions_h_

#include "clients_gui/TacticalLinePositions_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class CoordLatLongList;
    class LimitUpdate;
    class Location;
    class PhaseLineUpdate;
}

namespace kernel
{
    class Location_ABC;
    class LocationVisitor_ABC;
}

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public gui::TacticalLinePositions_ABC
                            , public kernel::Updatable_ABC< sword::PhaseLineUpdate >
                            , public kernel::Updatable_ABC< sword::LimitUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( kernel::Controller& controller, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter,
                                    const kernel::TacticalLine_ABC& owner );
             TacticalLinePositions( kernel::Controller& controller, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter,
                                    const kernel::TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::PhaseLineUpdate&  message );
    virtual void DoUpdate( const sword::LimitUpdate& message );
    void Update( const sword::CoordLatLongList& coordinates );
    //@}
};

#endif // __TacticalLinePositions_h_
