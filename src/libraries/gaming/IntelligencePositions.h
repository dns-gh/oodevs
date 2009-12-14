// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencePositions_h_
#define __IntelligencePositions_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Intelligence_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  IntelligencePositions
    @brief  IntelligencePositions
*/
// Created: SBO 2007-10-15
// =============================================================================
class IntelligencePositions : public kernel::Positions
                            , public kernel::Updatable_ABC< ASN1T_MsgIntelligenceCreation >
                            , public kernel::Updatable_ABC< ASN1T_MsgIntelligenceUpdate >
                            , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Intelligence_ABC& holder, Publisher_ABC& publisher );
    virtual ~IntelligencePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f, float adaptiveFactor = 1 ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    void Set( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencePositions( const IntelligencePositions& );            //!< Copy constructor
    IntelligencePositions& operator=( const IntelligencePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const ASN1T_MsgIntelligenceCreation& message );
    virtual void DoUpdate( const ASN1T_MsgIntelligenceUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Intelligence_ABC& holder_;
    Publisher_ABC& publisher_;
    geometry::Point2f position_;
    float height_;
    //@}
};

#endif // __IntelligencePositions_h_
