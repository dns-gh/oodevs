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

#include "Layer_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class DetectionMap;
    class GlTools_ABC;
    class GlTooltip_ABC;
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  MetricsLayer
    @brief  MetricsLayer
*/
// Created: AGE 2006-03-17
// =============================================================================
class MetricsLayer : public Layer_ABC
                   , public tools::Observer_ABC
                   , public kernel::OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MetricsLayer( kernel::Controllers& controllers, const kernel::DetectionMap& elevation, kernel::GlTools_ABC& tools  );
    virtual ~MetricsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove ( QMouseEvent* event, const geometry::Point2f& point );

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MetricsLayer( const MetricsLayer& );            //!< Copy constructor
    MetricsLayer& operator=( const MetricsLayer& ); //!< Assignement operator
    //@}

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
    float ComputeRuleDistance();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::GlTools_ABC& tools_;
    const kernel::DetectionMap& elevation_;
    std::auto_ptr< kernel::GlTooltip_ABC > tooltip_;
    bool multiRulingMode_;
    bool b3dComputation_;
    geometry::Point2f start_;
    geometry::Point2f end_;
    T_MetricPoints metricPoints_;
    //@}
};

}

#endif // __MetricsLayer_h_
