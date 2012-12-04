// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropagationAttribute_h_
#define __PropagationAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_array.hpp>
#include <vector>

namespace sword
{
    class ObjectUpdate;
    class ObjectCreation;
}

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
    class ObjectType;
    class DisasterTypes;
    class DisasterType;
}

class Simulation;
class Propagation;
class PropagationManager;

// =============================================================================
/** @class  PropagationAttribute
    @brief  Propagation attribute
*/
// Created: LGY 2012-10-12
// =============================================================================
class PropagationAttribute : public kernel::DisasterAttribute_ABC
                           , public kernel::Drawable_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             PropagationAttribute( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                                   const kernel::ObjectType& type, const kernel::DisasterTypes& disasterTypes );
    virtual ~PropagationAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropagationAttribute( const PropagationAttribute& );            //!< Copy constructor
    PropagationAttribute& operator=( const PropagationAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< Propagation > T_Propagation;
    typedef std::vector< T_Propagation >     T_Propagations;
    typedef T_Propagations::const_iterator CIT_Propagations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< PropagationManager > pManager_;
    const kernel::DisasterType& disasterType_;
    T_Propagations propagations_;
    std::vector< std::string > files_;
    //@}
};

#endif // __PropagationAttribute_h_
