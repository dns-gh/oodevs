// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __WeaponRangeLayer_h_
#define __WeaponRangeLayer_h_

#include "clients_gui/EntityLayer.h"
#include <boost/shared_ptr.hpp>

class Weapons;
class TesselatedShape;
typedef boost::shared_ptr< TesselatedShape > TesselatedShapePtr;

// =============================================================================
/** @class  WeaponRangeLayer
    @brief  WeaponRangeLayer
*/
// Created: MCO 2014-09-12
// =============================================================================
class WeaponRangeLayer : public gui::EntityLayerBase
                       , public tools::ElementObserver_ABC< kernel::Agent_ABC >
{
public:
             WeaponRangeLayer( kernel::Controllers& controllers,
                               gui::GLView_ABC& view,
                               gui::ColorStrategy_ABC& strategy,
                               const kernel::Profile_ABC& profile );
    virtual ~WeaponRangeLayer();

private:
    virtual void Paint( const geometry::Rectangle2f& extent );
    virtual void Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode );

    virtual void NotifyCreated( const kernel::Agent_ABC& entity );
    virtual void NotifyDeleted( const kernel::Agent_ABC& entity );

    void Reset();

private:
    typedef std::vector< geometry::Point2f > T_Positions;
    typedef std::vector< std::pair< unsigned int, unsigned int > > T_Ranges;
    typedef std::vector< TesselatedShapePtr > T_TesselatedShapes;

private:
    kernel::Controllers& controllers_;
    gui::ColorStrategy_ABC& strategy_;
    std::map< QString, T_Positions > positions_;
    std::map< QString, T_Ranges > ranges_;
    std::map< QString, T_TesselatedShapes > tesselated_;
    QColor color_;
};

#endif // __WeaponRangeLayer_h_
