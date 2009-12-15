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
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

class TacticalLine_ABC;

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public kernel::Positions
                            , public kernel::Serializable_ABC
                            , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
             TacticalLinePositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f, float adaptiveFactor = 1.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& ) const;

    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    void InsertPoint( const geometry::Point2f& point, float precision );
    void RemovePoint( const geometry::Point2f& point, float precision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLinePositions( const TacticalLinePositions& );            //!< Copy constructor
    TacticalLinePositions& operator=( const TacticalLinePositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    void ComputeBoundingBox();
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
