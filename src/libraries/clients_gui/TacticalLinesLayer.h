// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinesLayer_h_
#define __TacticalLinesLayer_h_

#include "EntityLayer.h"
#include "ShapeHandler_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class TacticalLine_ABC;
    struct Nothing;
}

namespace gui
{
    class ParametersLayer;
}

namespace gui
{
    class ModelObserver_ABC;
// =============================================================================
/** @class  TacticalLinesLayer
    @brief  TacticalLinesLayer
*/
// Created: AGE 2006-11-21
// =============================================================================
class TacticalLinesLayer : public EntityLayer< kernel::TacticalLine_ABC >
                         , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
                         , private ShapeHandler_ABC
                         , private kernel::LocationVisitor_ABC
                         , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinesLayer( kernel::Controllers& controllers,
                                 GlTools_ABC& tools,
                                 ColorStrategy_ABC& strategy,
                                 const std::shared_ptr< ParametersLayer >& parameters,
                                 View_ABC& view,
                                 const kernel::Profile_ABC& profile,
                                 ModelObserver_ABC& model );
    virtual ~TacticalLinesLayer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void CreateLimit( const T_PointVector& points ) = 0;
    virtual void CreateLima( const T_PointVector& points ) = 0;
    virtual bool CanCreateLine();

    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void FillContextMenu( const kernel::GraphicalEntity_ABC& entity, kernel::ContextMenu& menu );

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual void NotifyContextMenu     ( const kernel::Nothing&, kernel::ContextMenu& menu );
    virtual void NotifySelectionChanged( const std::vector< const kernel::TacticalLine_ABC* >& elements );
    virtual void OptionChanged         ( const std::string& /*name*/, const kernel::OptionVariant& /*value*/ ) {}

    virtual void Handle( kernel::Location_ABC& location );

    virtual void VisitLines    ( const T_PointVector& points );
    virtual void VisitRectangle( const T_PointVector& /*points*/ ) {};
    virtual void VisitPolygon  ( const T_PointVector& ) {};
    virtual void VisitPath     ( const geometry::Point2f& , const T_PointVector& ) {};
    virtual void VisitCircle   ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint    ( const geometry::Point2f& ) {};
    virtual void VisitCurve    ( const T_PointVector& ) {};
    virtual void VisitText     ( const QString&, const QFont&, const geometry::Point2f& ) {}
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateLimit();
    void OnCreateLima ();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const GlTools_ABC& tools_;
    gui::ColorStrategy_ABC& strategy_;
    std::shared_ptr< gui::ParametersLayer > parameters_;
    ModelObserver_ABC& model_;
    bool isLimit_;
    bool isEditing_;
    kernel::SafePointer< kernel::TacticalLine_ABC > selected_;
    //@}
};

}

#endif // __TacticalLinesLayer_h_
