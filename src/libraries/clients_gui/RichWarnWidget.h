// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichWarnWidget_h_
#define __RichWarnWidget_h_

#include "RichBaseObject.h"
#include "RichWidget.h"
#include "RichWarnWidget_ABC.h"

namespace gui
{

// =============================================================================
/** @class  RichWarnWidget
    @brief  RichWarnWidget
*/
// Created: ABR 2013-06-10
// =============================================================================
template< typename T >
class RichWarnWidget : public RichWidget< T >
                     , public RichWarnWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RichWarnWidget( const QString& objectName, QWidget* parent = 0 );
    virtual ~RichWarnWidget();
    //@}

    //! @name Operations
    //@{
    virtual void EnableStaticWarning( bool warn );
    virtual void Warn( int msec = 3000 );
    virtual QPalette CreatePalette() const;
    //@}

    //! @name RichWarnWidget_ABC implementation
    //@{
    virtual void OnBlink();
    virtual void OnDone();
    //@}

private:
    //! @name Member data
    //@{
    RichBaseObject* baseWidget_;
    QTimer* timer_;
    QPalette originalPalette_;
    QPalette warningPalette_;
    bool blink_;
    //@}
};

#include "RichWarnWidget.inl"

} //! namespace gui

#endif // __RichWarnWidget_h_
