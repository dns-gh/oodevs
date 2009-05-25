// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "LocationSerializer.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class ParametersLayer;
    class RichLabel;
    class LocationCreator;
}

namespace actions
{
    class Parameter_ABC;
}

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public Param_ABC, private gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void Handle( kernel::Location_ABC& location );
    void SetShapeFilter( bool point, bool line, bool polygon, bool circle );

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename T >
    T* CreateParameter() const
    {
        if( ! location_.get() )
        {
            if( IsOptional() )
                return 0;
            throw std::runtime_error( "Entity not set!" );
        }
        return new T( parameter_, converter_, *location_ );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamLocation( const ParamLocation& );
    ParamLocation& operator=( const ParamLocation& );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

    //! @name Types
    //@{
    struct ShapeFilter
    {
        ShapeFilter( bool point, bool line, bool polygon, bool circle )
            : point_( point ), line_( line ), polygon_( polygon ), circle_( circle ) {}
        bool point_, line_, polygon_, circle_;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::OrderParameter parameter_;
    gui::ParametersLayer& layer_;
    gui::LocationCreator* creator_;
    gui::RichLabel* pLabel_;
    QLabel* pShapeLabel_;
    std::auto_ptr< kernel::Location_ABC > location_;
    kernel::ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    ShapeFilter filter_;
    //@}
};

#endif // __ParamLocation_h_
