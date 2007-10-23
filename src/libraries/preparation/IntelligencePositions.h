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

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  IntelligencePositions
    @brief  IntelligencePositions
*/
// Created: SBO 2007-10-15
// =============================================================================
class IntelligencePositions : public kernel::Positions
                            , public kernel::Drawable_ABC
                            , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencePositions( const kernel::CoordinateConverter_ABC& converter, const geometry::Point2f& position );
             IntelligencePositions( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~IntelligencePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
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
    virtual void SerializeIntelligences( xml::xostream& ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    float height_;
    //@}
};

#endif // __IntelligencePositions_h_
