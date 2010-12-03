// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantPositions_h_
#define __InhabitantPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "geometry/types.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class InhabitantType;
    class GlTools_ABC;
}

namespace gui
{
    class TerrainObjectProxy;
}

namespace xml
{
    class xistream;
}

class UrbanModel;

// =============================================================================
/** @class  InhabitantPositions
@brief  InhabitantPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class InhabitantPositions : public kernel::Positions
                          , public kernel::Serializable_ABC

{
public:
    //! @name Constructors/Destructor
    //@{
    InhabitantPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, UrbanModel& urbanModel );
    InhabitantPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, UrbanModel& urbanModel );
    virtual ~InhabitantPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;

    virtual void SerializeAttributes( xml::xostream& xos ) const;

    void Draw( const kernel::GlTools_ABC& tools ) const;

    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantPositions( const InhabitantPositions& );            //!< Copy constructor
    InhabitantPositions& operator=( const InhabitantPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis );
    void ReadLivingUrbanBlock( xml::xistream& xis );
    //@}

    typedef std::vector< const gui::TerrainObjectProxy* > T_UrbanObjectVector;
    typedef T_UrbanObjectVector::iterator          IT_UrbanObjectVector;
    typedef T_UrbanObjectVector::const_iterator   CIT_UrbanObjectVector;

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Location_ABC* location_;
    UrbanModel& urbanModel_;
    T_UrbanObjectVector livingUrbanObject_;
    //@}

};

#endif // __InhabitantPositions_h_
