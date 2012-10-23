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
#include "clients_kernel/ASCExtractor.h"
#include "tools/ElementObserver_ABC.h"
#include "protocol/Protocol.h"
#include <boost/filesystem/path.hpp>

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
    void ReadFile( xml::xistream& xis );
    void ReadColor( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string >     T_Files;
    typedef std::map< QDateTime, T_Files > T_Propagation;
    typedef T_Propagation::iterator       IT_Propagation;

    typedef std::map< double, QColor > T_Colors;
    typedef T_Colors::const_iterator CIT_Colors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    boost::filesystem::path path_;
    std::string projection_;
    T_Propagation propagation_;
    T_Colors colors_;
    kernel::ASCExtractor::T_Tiles tiles_;
    //@}
};

#endif // __PropagationAttribute_h_
