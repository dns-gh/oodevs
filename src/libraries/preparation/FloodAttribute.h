// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodAttribute_h_
#define __FloodAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"
#include "propagation/ElevationGetter_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace propagation
{
    class FloodDrawer;
    class FloodModel_ABC;
}

namespace kernel
{
    class AltitudeModified;
    class Controllers;
    class DetectionMap;
    class Positions;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-07
// =============================================================================
class FloodAttribute : public kernel::FloodAttribute_ABC
                     , public kernel::Serializable_ABC
                     , public gui::Drawable_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< FloodAttribute >
                     , public tools::ElementObserver_ABC< kernel::AltitudeModified >
                     , public propagation::ElevationGetter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodAttribute( gui::PropertiesDictionary& dico, const kernel::DetectionMap& detection, const kernel::Positions& positions, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
             FloodAttribute( xml::xistream& xis, const kernel::DetectionMap& detection, const kernel::Positions& positions, gui::PropertiesDictionary& dico, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
    virtual ~FloodAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void NotifyUpdated( const FloodAttribute& attribute );
    virtual void NotifyUpdated( const kernel::AltitudeModified& attribute );
    virtual double GetElevationAt( const geometry::Point2f& point ) const;
    virtual float GetCellSize() const;
    //@}

    //! @name Operations
    //@{
    void SetValues( int depth, int refDist );
    void EndDrag() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    const kernel::Positions& positions_;
    kernel::UnitedValue< int > depth_;
    kernel::UnitedValue< int > refDist_;
    std::unique_ptr< propagation::FloodModel_ABC > floodModel_;
    std::unique_ptr< propagation::FloodDrawer > floodDrawer_;
    //@}
};

#endif // __FloodAttribute_h_
