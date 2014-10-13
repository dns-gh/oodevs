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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "InhabitantPositions_ABC.h"
#include <geometry/Types.h>
#include <boost/tuple/tuple.hpp>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class Inhabitant_ABC;
    class Controller;
    class UrbanObject_ABC;
}

namespace gui
{
    class GLView_ABC;
    class PropertiesDictionary;
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
                          , public gui::Drawable_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{
public:
    //! @name Types
    //@{
    struct T_UrbanObject
    {
        T_UrbanObject( unsigned long id, const std::string name, const kernel::UrbanObject_ABC& urban )
            : id_( id ), name_( name ), pUrban_( &urban ) {}
        unsigned long id_;
        std::string name_;
        const kernel::UrbanObject_ABC* pUrban_;
    };
    typedef std::vector< T_UrbanObject > T_UrbanObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location,
                                  const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant, gui::PropertiesDictionary& dictionary );
             InhabitantPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const UrbanModel& urbanModel,
                                  kernel::Inhabitant_ABC& inhabitant, gui::PropertiesDictionary& dictionary );
    virtual ~InhabitantPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Add( const kernel::Location_ABC& location );
    virtual void Remove( const kernel::Location_ABC& location );

    void StartEdition();
    void Update( const geometry::Point2f& point );
    void Accept();
    void Reject();

    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& block );
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& block );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadLivingUrbanBlock( xml::xistream& xis );
    void ComputePosition();
    void UpdateDictionary();
    void Add( const kernel::UrbanObject_ABC& object, const geometry::Polygon2f& polygon );
    void Remove( const kernel::UrbanObject_ABC& object, const geometry::Polygon2f& polygon );
    bool Exists( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const UrbanModel& urbanModel_;
    kernel::Inhabitant_ABC& inhabitant_;
    gui::PropertiesDictionary& dictionary_;
    geometry::Point2f position_;
    T_UrbanObjectVector livingUrbanObject_;
    T_UrbanObjectVector edition_;
    unsigned int livingUrbanObjects_;
    unsigned int nominalCapacity_;
    unsigned int infrastructures_;
    unsigned int medicalInfrastructures_;
    QMap< QString, unsigned int > accomodationCapacties_;
    //@}

};

#endif // __InhabitantPositions_h_
