// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinesLayer_h_
#define __TacticalLinesLayer_h_

#include "EntityLayer.h"
#include "ShapeHandler_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class TacticalLine_ABC;
    struct Nothing;
}

namespace gui
{
    class ParametersLayer;

// =============================================================================
/** @class  TacticalLinesLayer
    @brief  TacticalLinesLayer
*/
// Created: AGE 2006-11-21
// =============================================================================
class TacticalLinesLayer : public QObject
                         , public EntityLayer< kernel::TacticalLine_ABC >
                         , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
                         , public kernel::OptionsObserver_ABC
                         , private ShapeHandler_ABC
                         , private kernel::LocationVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~TacticalLinesLayer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Delete( const kernel::TacticalLine_ABC& line ) = 0;
    virtual void CreateLimit( const T_PointVector& points ) = 0;
    virtual void CreateLima( const T_PointVector& points ) = 0;
    virtual bool CanCreateLine();
    virtual bool MouseMove       ( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool MousePress      ( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool MouseDoubleClick( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point );

    virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void NotifyContextMenu     ( const kernel::Nothing&, kernel::ContextMenu& menu );
    virtual void NotifySelected        ( const kernel::TacticalLine_ABC* element );

    virtual void Handle( kernel::Location_ABC& location );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitPath   ( const geometry::Point2f& , const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& ) {};
    //@}
    
private slots:
    //! @name Slots
    //@{
    void OnCreateLimit();
    void OnCreateLima ();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLinesLayer( const TacticalLinesLayer& );            //!< Copy constructor
    TacticalLinesLayer& operator=( const TacticalLinesLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    gui::ColorStrategy_ABC& strategy_;
    gui::ParametersLayer& parameters_;

    kernel::FourStateOption drawLines_;
    bool isLimit_;
    kernel::SafePointer< kernel::TacticalLine_ABC > selected_;
    //@}
};

}

#endif // __TacticalLinesLayer_h_
