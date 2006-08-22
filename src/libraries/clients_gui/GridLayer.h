// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GridLayer_h_
#define __GridLayer_h_

#include "Layer_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
class GlTools_ABC;
class Controllers;

// =============================================================================
/** @class  GridLayer
    @brief  GridLayer
*/
// Created: AGE 2006-08-22
// =============================================================================
class GridLayer : public Layer2d_ABC
                , public Observer_ABC
                , public OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GridLayer( Controllers& controllers, GlTools_ABC& tools );
    virtual ~GridLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GridLayer( const GridLayer& );            //!< Copy constructor
    GridLayer& operator=( const GridLayer& ); //!< Assignement operator
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
    //@}
};

#endif // __GridLayer_h_
