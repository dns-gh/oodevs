// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ParamLocation_ABC_h_
#define __ParamLocation_ABC_h_

#include "Param_ABC.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "actions/Location.h"
#include "actions/ParameterContainer_ABC.h"
#include "actions/Path.h"
#include "actions/Point.h"
#include "actions/Polygon.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Positions;
    struct Nothing;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamLocation_ABC
    @brief  ParamLocation_ABC
*/
// Created: ABR 2012-01-03
// =============================================================================
template< typename BaseParameter >
class ParamLocation_ABC : public Param_ABC
                        , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                        , protected ::gui::ShapeHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation_ABC( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamLocation_ABC();
    //@}

    //! @name Operations
    //@{
    void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void OnMenuClick();
    //@}

private:
    //! @name Helpers
    //@{
    void InternalOnMenuClick();
    virtual bool InternalCheckValidity() const;
    //@}

protected:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    ::gui::ParametersLayer&                layer_;
    QLabel*                                pPosLabel_;
    std::auto_ptr< kernel::Location_ABC >  location_;
    //@}
};

#include "ParamLocation_ABC.inl"

typedef ParamLocation_ABC< actions::parameters::Point >     ParamPoint;
typedef ParamLocation_ABC< actions::parameters::Polygon >   ParamPolygon;

typedef ParamLocation_ABC< actions::parameters::Location >  ParamCircle;
typedef ParamLocation_ABC< actions::parameters::Location >  ParamLine;
typedef ParamLocation_ABC< actions::parameters::Location >  ParamRectangle;

    }
}

#endif // __ParamLocation_ABC_h_
