// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichSlider_h_
#define __RichSlider_h_

#include "RichWidget.h"
#include "QtGui/qslider.h"

namespace gui
{
// =============================================================================
/** @class  RichSlider
    @brief  RichSlider
*/
// Created: NPT 2013-07-05
// =============================================================================
class RichSlider : public RichWidget< QSlider >
{

public:
    //! @name Constructors/Destructor
    //@{
             RichSlider( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichSlider();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

} //! namespace gui

#endif // __RichSlider_h_
