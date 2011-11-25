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
    virtual void Handle( kernel::Location_ABC& location );
    void SetShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetName( const QString& name );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename Concrete >
    void Commit( actions::ParameterContainer_ABC& action ) const
    {
        if( IsChecked() && location_.get() && location_->IsValid() )
            action.AddParameter( *new Concrete( parameter_, converter_, *location_ ) );
        else
            action.AddParameter( *new Concrete( parameter_, converter_ ) );
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
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
    QLabel* pShapeLabel_;
    std::auto_ptr< kernel::Location_ABC > location_;
    kernel::ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    ShapeFilter filter_;
    //@}
};

    }
}

#endif // __ParamLocation_h_
