// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogoLayer_h_
#define __LogoLayer_h_

#include "Layer_ABC.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{

// =============================================================================
/** @class  LogoLayer
    @brief  LogoLayer
*/
// Created: AGE 2007-02-12
// =============================================================================
class LogoLayer : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogoLayer( const kernel::GlTools_ABC& tools, const QImage& image );
    virtual ~LogoLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogoLayer( const LogoLayer& );            //!< Copy constructor
    LogoLayer& operator=( const LogoLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    QImage image_;
    //@}
};

}

#endif // __LogoLayer_h_
