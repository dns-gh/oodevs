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
    class DetectionMap;
}

namespace gui
{
    class GlTools_ABC;
    class GlTooltip_ABC;
}

namespace gui
{
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
    explicit MetricsLayer( const kernel::DetectionMap& elevation, gui::GlTools_ABC& tools  );
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
    //! @name Types
    //@{
    typedef std::vector< geometry::Point2f >        T_MetricPoints;
    typedef T_MetricPoints::iterator                IT_MetricPoints;
    typedef T_MetricPoints::const_iterator          CIT_MetricPoints;
    //@}

private:
    //! @name Helpers
    //@{
    float ComputeRuleDistance( bool b3dComputation );
    //@}

private:
    //! @name Member data
    //@{
    GlTools_ABC& tools_;
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< GlTooltip_ABC > tooltip_;
    bool multiRulingMode_;
    geometry::Point2f start_;
    geometry::Point2f end_;
    T_MetricPoints metricPoints_;
    //@}
};

}

#endif // __MetricsLayer_h_
