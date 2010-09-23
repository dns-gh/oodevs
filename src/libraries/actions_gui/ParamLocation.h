// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ShapeHandler_ABC.h"

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

    namespace gui
    {

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public Param_ABC
                    , private ::gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void Handle( kernel::Location_ABC& location );
    void SetShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle );

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename Concrete, typename Stub >
    void Commit( actions::ParameterContainer_ABC& action ) const
    {
        std::auto_ptr< actions::Parameter_ABC > param;
        if( location_.get() )
        {
            param.reset( new Concrete( parameter_, converter_, *location_ ) );
            param->Set( location_->IsValid() );
        }
        else
        {
            Stub stub;
            param.reset( new Concrete( parameter_, converter_, stub ) );
            param->Set( false );
        }
        action.AddParameter( *param.release() );
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
        ShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle )
            : point_( point ), line_( line ), polygon_( polygon ), circle_( circle ), rectangle_( rectangle ) {}
        bool point_, line_, polygon_, circle_, rectangle_;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::OrderParameter parameter_;
    ::gui::ParametersLayer& layer_;
    ::gui::LocationCreator* creator_;
    ::gui::RichLabel* pLabel_;
    QLabel* pShapeLabel_;
    std::auto_ptr< kernel::Location_ABC > location_;
    kernel::ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    ShapeFilter filter_;
    //@}
};

    }
}

#endif // __ParamLocation_h_
