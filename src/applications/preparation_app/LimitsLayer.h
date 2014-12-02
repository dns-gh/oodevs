// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsLayer_h_
#define __LimitsLayer_h_

#include "clients_gui/TacticalLinesLayer.h"
#include "clients_kernel/FourStateOption.h"

namespace gui
{
    class ExclusiveEventStrategy;
}

class ModelBuilder;

// =============================================================================
/** @class  LimitsLayer
    @brief  LimitsLayer
*/
// Created: AGE 2006-03-24
// =============================================================================
class LimitsLayer : public gui::TacticalLinesLayer
                  , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( kernel::Controllers& controllers,
                          gui::GLView_ABC& view,
                          gui::ColorStrategy_ABC& strategy,
                          const std::shared_ptr< gui::ParametersLayer >& parameters,
                          ModelBuilder& modelBuilder,
                          const kernel::Profile_ABC& profile );
    virtual ~LimitsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& line, kernel::ContextMenu& menu );

    virtual bool CanCreateLine();
    virtual void CreateLimit( const T_PointVector& points );
    virtual void CreateLima( const T_PointVector& points );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );

    float Precision( const geometry::Point2f& point ) const;
    bool IsEligibleForDrag() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEdit();
    //@}

private:
    //! @name Member data
    //@{
    ModelBuilder& modelBuilder_;
    geometry::Point2f dragPoint_;
    geometry::Point2f oldPosition_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __LimitsLayer_h_
