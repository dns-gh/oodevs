// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichWidget_h_
#define __RichWidget_h_

#include "ObjectNameManager.h"

namespace gui
{

// =============================================================================
/** @class  RichWidget
    @brief  RichWidget
*/
// Created: ABR 2013-06-07
// =============================================================================
template< typename T >
class RichWidget : public T
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichWidget( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichWidget();
    //@}
};

#include "RichWidget.inl"

} //! namespace gui

#endif // __RichWidget_h_
