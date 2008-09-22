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

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "clients_kernel/LocationVisitor_ABC.h"

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
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Location& asn );
             Path( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
             Path( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Path();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_Path& asn ) const;
    void Clean( ASN1T_Path& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Path( const Path& );            //!< Copy constructor
    Path& operator=( const Path& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {}
    virtual void VisitCircle ( const geometry::Point2f&, float ) {}
    virtual void VisitPoint  ( const geometry::Point2f& ) {}
    virtual void VisitPath   ( const geometry::Point2f& first, const T_PointVector& points );
    void AddPoints( const ASN1T_Location& asn );
    void ReadPoint( xml::xistream& xis );
    void AddPoint( const geometry::Point2f& p, unsigned i, unsigned count );
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
