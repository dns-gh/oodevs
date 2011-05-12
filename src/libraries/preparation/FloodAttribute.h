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

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"
#include "flood/ElevationGetter_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace flood
{
    class FloodDrawer;
    class FloodModel;
}

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class Positions;
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
                     , public kernel::Drawable_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< FloodAttribute >
                     , public flood::ElevationGetter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodAttribute( kernel::PropertiesDictionary& dico, const kernel::DetectionMap& detection, const kernel::Positions& positions, kernel::Controllers& controllers );
             FloodAttribute( xml::xistream& xis, const kernel::DetectionMap& detection, const kernel::Positions& positions, kernel::PropertiesDictionary& dico, kernel::Controllers& controllers );
    virtual ~FloodAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void NotifyUpdated( const FloodAttribute& attribute );
    virtual short GetElevationAt( const geometry::Point2f& point ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetValues( int depth, int refDist );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodAttribute( const FloodAttribute& );            //!< Copy constructor
    FloodAttribute& operator=( const FloodAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    const kernel::Positions& positions_;
    kernel::UnitedValue< int > depth_;
    kernel::UnitedValue< int > refDist_;
    std::auto_ptr< flood::FloodModel > floodModel_;
    std::auto_ptr< flood::FloodDrawer > floodDrawer_;
    //@}
};

#endif // __FloodAttribute_h_
