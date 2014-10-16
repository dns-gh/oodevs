// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationDetections_h_
#define __PopulationDetections_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Population_ABC;
    class PopulationPart_ABC;
    class Entity_ABC;
    class CoordinateConverter_ABC;
}

namespace sword
{
    class CrowdConcentrationDetection;
    class CrowdFlowDetection;
}

// =============================================================================
/** @class  PopulationDetections
    @brief  PopulationDetections
*/
// Created: AGE 2006-02-27
// =============================================================================
class PopulationDetections : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< sword::CrowdConcentrationDetection >
                           , public kernel::Updatable_ABC< sword::CrowdFlowDetection >
                           , public gui::Drawable_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::PopulationPart_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationDetections( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::Population_ABC >& resolver, const kernel::Entity_ABC& entity );
    virtual ~PopulationDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationDetections( const PopulationDetections& );            //!< Copy constructor
    PopulationDetections& operator=( const PopulationDetections& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::CrowdConcentrationDetection& message );
    virtual void DoUpdate( const sword::CrowdFlowDetection& message );
    virtual void NotifyDeleted( const kernel::PopulationPart_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::set< const kernel::PopulationPart_ABC* > T_Parts;
    typedef T_Parts::const_iterator                     CIT_Parts;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver_ABC< kernel::Population_ABC >& resolver_;
    const kernel::Entity_ABC& entity_;
    T_Parts perceived_;
    //@}
};

#endif // __PopulationDetections_h_
