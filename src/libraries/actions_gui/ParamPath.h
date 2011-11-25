// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPath_h_
#define __ParamPath_h_

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ShapeHandler_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class Location_ABC;
    class Positions;
    struct Nothing;
}

namespace gui
{
    class ParametersLayer;
    class RichLabel;
}

namespace actions
{
    class Parameter_ABC;

    namespace gui
    {

// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public QObject
                , public Param_ABC
                , public kernel::ContextMenuObserver_ABC< kernel::Nothing > // $$$$ AGE 2007-07-11: geometry::Point2d
                , private ::gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamPath();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void StartPath();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    ::gui::ParametersLayer& layer_;
    const kernel::Entity_ABC& entity_;
    QLabel*               pPosLabel_;
    std::auto_ptr< kernel::Location_ABC > location_;
    //@}
};

    }
}

#endif // __ParamPath_h_
