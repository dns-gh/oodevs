// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MetricsLayer_h_
#define __MetricsLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
    class DetectionMap;
}

namespace gui
{
    class InformationToolTip;

// =============================================================================
/** @class  MetricsLayer
    @brief  MetricsLayer
*/
// Created: AGE 2006-03-17
// =============================================================================
class MetricsLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MetricsLayer( kernel::Controllers& controllers,
                           const kernel::DetectionMap& elevation,
                           gui::GLView_ABC& tools );
    virtual ~MetricsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove ( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    void DrawAngle() const;
    float ComputeAngle() const;
    //@}

private:
    //! @name Helpers
    //@{
    float ComputeRuleDistance( bool b3dComputation );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< InformationToolTip > tooltip_;
    bool multiRulingMode_;
    geometry::Point2f start_;
    geometry::Point2f end_;
    std::vector< geometry::Point2f > metricPoints_;
    //@}
};

}

#endif // __MetricsLayer_h_
