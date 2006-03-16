// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlWidget_h_
#define __GlWidget_h_

#include "graphics/MapWidget.h"
#include "WorldParameters.h"

class CoordinateConverter;
class Controller;
class ActionController;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private WorldParameters, public MapWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent, const std::string& scipioXml,
                       Controller& controller, ActionController& actions, const CoordinateConverter& converter );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlWidget( const GlWidget& );            //!< Copy constructor
    GlWidget& operator=( const GlWidget& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __GlWidget_h_
