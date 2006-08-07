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
#include "astec_gaming/Observer_ABC.h"
#include "astec_gaming/OptionsObserver_ABC.h"
class GlTools_ABC;
class Controllers;

// =============================================================================
/** @class  MetricsLayer
    @brief  MetricsLayer
*/
// Created: AGE 2006-03-17
// =============================================================================
class MetricsLayer : public Layer2d_ABC
                   , public Observer_ABC
                   , public OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MetricsLayer( Controllers& controllers, GlTools_ABC& tools );
    virtual ~MetricsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
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

    //! @name Helpers
    //@{
    float Displace( float value );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    GlTools_ABC& tools_;

    geometry::Rectangle2f extent_;
    float gridSize_;

    bool ruling_;
    geometry::Point2f start_;
    geometry::Point2f end_;
    //@}
};

#endif // __MetricsLayer_h_
