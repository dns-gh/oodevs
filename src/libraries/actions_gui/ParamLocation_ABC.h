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
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Rectangle.h"
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
                        , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
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
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& extent, ::gui::GlTools_ABC& tools ) const;
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void OnMenuClick();
    virtual void Visit( const actions::parameters::Point& param );
    virtual void Visit( const actions::parameters::Polygon& param );
    virtual void Visit( const actions::parameters::Location& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    void InternalVisit( const actions::parameters::Location& param );
    //@}

protected:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC&  converter_;
    ::gui::ParametersLayer&                 layer_;
    QLabel*                                 pPosLabel_;
    std::unique_ptr< kernel::Location_ABC > location_;
    std::unique_ptr< geometry::Point2f >    popupPosition_;
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
