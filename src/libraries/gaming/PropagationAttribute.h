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
}

class Simulation;
class Propagation;

// =============================================================================
/** @class  PropagationAttribute
    @brief  Propagation attribute
*/
// Created: LGY 2012-10-12
// =============================================================================
class PropagationAttribute : public kernel::InputPropagationAttribute_ABC
                           , public kernel::Drawable_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             PropagationAttribute( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
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
    void ReadFile( xml::xistream& xis, const boost::filesystem::path& path );
    void ReadColor( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string >           T_Files;
    typedef std::map< QDateTime, T_Files >       T_PropagationFiles;
    typedef T_PropagationFiles::const_iterator CIT_PropagationFiles;

    typedef std::map< double, QColor > T_Colors;
    typedef T_Colors::const_iterator CIT_Colors;

    typedef boost::shared_ptr< Propagation > T_Propagation;
    typedef std::vector< T_Propagation >     T_Propagations;
    typedef T_Propagations::const_iterator CIT_Propagations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    std::string projection_;
    T_PropagationFiles propagationFiles_;
    T_Colors colors_;
    T_Propagations propagations_;
    QDateTime last_;
    //@}
};

#endif // __PropagationAttribute_h_
