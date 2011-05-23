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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "flood/ElevationGetter_ABC.h"

namespace flood
{
    class FloodDrawer;
    class FloodModel;
}

namespace kernel
{
    class Controller;
    class DetectionMap;
    class Positions;
}

class Simulation;

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodAttribute : public kernel::FloodAttribute_ABC
                     , public kernel::Drawable_ABC
                     , public flood::ElevationGetter_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodAttribute( kernel::Controller& controller, const kernel::DetectionMap& detection, const kernel::Positions& positions );
    virtual ~FloodAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual short GetElevationAt( const geometry::Point2f& point ) const;
    virtual void NotifyUpdated( const Simulation& simulation );
    bool ReadFromODB() const;
    void GenerateFlood( bool force = false );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodAttribute( const FloodAttribute& );            //!< Copy constructor
    FloodAttribute& operator=( const FloodAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DetectionMap& detection_;
    const kernel::Positions& positions_;
    std::auto_ptr< flood::FloodModel > floodModel_;
    std::auto_ptr< flood::FloodDrawer > floodDrawer_;
    bool floodGenerated_;
    bool readFromODB_;
    int depth_;
    int refDist_;
    //@}
};

#endif // __FloodAttribute_h_
