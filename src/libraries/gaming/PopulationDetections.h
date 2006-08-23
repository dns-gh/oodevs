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

#include "DIN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Population_ABC;
    class PopulationPart_ABC;
}

// =============================================================================
/** @class  PopulationDetections
    @brief  PopulationDetections
*/
// Created: AGE 2006-02-27
// =============================================================================
class PopulationDetections : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< ConcentrationDetectionMessage >
                           , public kernel::Updatable_ABC< FlowDetectionMessage >
                           , public kernel::Updatable_ABC< PopulationCollisionMessage >
                           , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationDetections( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Population_ABC >& resolver );
    virtual ~PopulationDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationDetections( const PopulationDetections& );            //!< Copy constructor
    PopulationDetections& operator=( const PopulationDetections& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ConcentrationDetectionMessage& message );
    virtual void DoUpdate( const FlowDetectionMessage& message );
    virtual void DoUpdate( const PopulationCollisionMessage& message );
    //@}

    //! @name Types
    //@{
    typedef std::set< const kernel::PopulationPart_ABC* > T_Parts;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Population_ABC >& resolver_;
    T_Parts perceived_;
    T_PointVector shape_;
    //@}
};

#endif // __PopulationDetections_h_
