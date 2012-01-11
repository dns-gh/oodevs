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

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "InhabitantPositions_ABC.h"
#include "geometry/types.h"
#include <boost/tuple/tuple.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class GlTools_ABC;
    class Inhabitant_ABC;
    class PropertiesDictionary;
    class Controller;
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
// Created: SLG 2010-11-25
// =============================================================================
class InhabitantPositions : public InhabitantPositions_ABC
                          , public kernel::Serializable_ABC
                          , public kernel::Drawable_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::tuple< unsigned long, std::string, const gui::TerrainObjectProxy* > T_UrbanObject;
    typedef std::vector< T_UrbanObject >                                               T_UrbanObjectVector;
    typedef T_UrbanObjectVector::iterator                                             IT_UrbanObjectVector;
    typedef T_UrbanObjectVector::const_iterator                                      CIT_UrbanObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location,
                                  const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dictionary );
             InhabitantPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const UrbanModel& urbanModel,
                                  kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dictionary );
    virtual ~InhabitantPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Add( const kernel::Location_ABC& location );
    virtual void Remove( const kernel::Location_ABC& location );

    void StartEdition();
    void Reject();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadLivingUrbanBlock( xml::xistream& xis );
    void ComputePosition();
    void UpdateDictionary();
    void Add( const gui::TerrainObjectProxy& object, const geometry::Polygon2f& polygon );
    void Remove( const gui::TerrainObjectProxy& object, const geometry::Polygon2f& polygon );
    bool Exists( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const UrbanModel& urbanModel_;
    kernel::Inhabitant_ABC& inhabitant_;
    kernel::PropertiesDictionary& dictionary_;
    geometry::Point2f position_;
    T_UrbanObjectVector livingUrbanObject_;
    T_UrbanObjectVector edition_;
    //@}

};

#endif // __InhabitantPositions_h_
