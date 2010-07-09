// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinePositions_h_
#define __TacticalLinePositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"

using namespace Common;


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
                            , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimaUpdate >
                            , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgLimitUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
             TacticalLinePositions( const Common::MsgLocation& message, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLinePositions( const TacticalLinePositions& );            //!< Copy constructor
    TacticalLinePositions& operator=( const TacticalLinePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimaUpdate&  message );
    virtual void DoUpdate( const MsgsMessengerToClient::MsgLimitUpdate& message );
    void Update( const Common::MsgTacticalLine& message );
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
