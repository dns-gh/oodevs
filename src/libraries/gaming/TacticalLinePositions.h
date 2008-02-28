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

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

class TacticalLine_ABC;

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public kernel::Positions
                            , public kernel::Drawable_ABC
                            , public kernel::Updatable_ABC< ASN1T_MsgLimaUpdate >
                            , public kernel::Updatable_ABC< ASN1T_MsgLimitUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
             TacticalLinePositions( const ASN1T_Line& asnMsg, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    void WriteGeometry( ASN1T_Line& line ) const; // $$$$ SBO 2006-11-06: void Accept( PositionVisitor_ABC& ) or something...
    void WriteGeometry( kernel::Location_ABC& location ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLinePositions( const TacticalLinePositions& );            //!< Copy constructor
    TacticalLinePositions& operator=( const TacticalLinePositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgLimaUpdate&  message );
    virtual void DoUpdate( const ASN1T_MsgLimitUpdate& message );
    void Update( const ASN1T_TacticalLine& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const TacticalLine_ABC& owner_;
    T_PointVector  pointList_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

#endif // __TacticalLinePositions_h_
