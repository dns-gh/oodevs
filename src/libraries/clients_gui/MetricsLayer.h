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

namespace kernel
{
    class GlTools_ABC;
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
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MetricsLayer( kernel::GlTools_ABC& tools );
    virtual ~MetricsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove ( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MetricsLayer( const MetricsLayer& );            //!< Copy constructor
    MetricsLayer& operator=( const MetricsLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::GlTools_ABC& tools_;
    bool ruling_;
    geometry::Point2f start_;
    geometry::Point2f end_;
    //@}
};

}

#endif // __MetricsLayer_h_
