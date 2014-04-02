// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPath_h_
#define __ActionParameterPath_h_

#include "Parameter.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace sword
{
    class Location;
}

namespace kernel
{
    class OrderParameter;
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Path
    @brief  Path
*/
// Created: SBO 2007-04-26
// =============================================================================
class Path : public Parameter< QString >
           , public kernel::LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Location& message );
    virtual ~Path();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( sword::Location& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual bool IsSet() const;
    const T_PointVector& GetPoints() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const ::gui::Viewport_ABC& viewport, const ::gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void VisitLines     ( const T_PointVector& points );
    virtual void VisitRectangle ( const T_PointVector& ) {};
    virtual void VisitPolygon   ( const T_PointVector& ) {}
    virtual void VisitCircle    ( const geometry::Point2f&, float ) {}
    virtual void VisitPoint     ( const geometry::Point2f& ) {}
    virtual void VisitPath      ( const geometry::Point2f& first, const T_PointVector& points );
    virtual void VisitCurve     ( const T_PointVector& points );
    void AddPoints( const sword::Location& message );
    void AddPoint( const geometry::Point2f& p, std::size_t i, std::size_t count );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};
    }
}

#endif // __ActionParameterPath_h_
