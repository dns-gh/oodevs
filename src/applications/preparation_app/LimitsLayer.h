// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsLayer_h_
#define __LimitsLayer_h_

#include "clients_gui/TacticalLinesLayer.h"
#include "clients_kernel/FourStateOption.h"
#include "EditorProxy.h"

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
class LimitsLayer : public EditorProxy< gui::TacticalLinesLayer >
{
public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view,
                          gui::ExclusiveEventStrategy& eventStrategy, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~LimitsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool CanCreateLine();
    virtual void Delete( const kernel::TacticalLine_ABC& line );
    virtual void CreateLimit( const T_PointVector& points );
    virtual void CreateLima( const T_PointVector& points );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    float Precision( const geometry::Point2f& point ) const;
    virtual bool MouseMove ( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool MousePress( kernel::TacticalLine_ABC& entity, QMouseEvent* mouse, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    ModelBuilder& modelBuilder_;
    gui::ExclusiveEventStrategy& eventStrategy_;
    const kernel::GlTools_ABC& tools_;
    geometry::Point2f dragPoint_;
    kernel::FourStateOption drawLines_;
    //@}
};

#endif // __LimitsLayer_h_
