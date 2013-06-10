// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichBaseObject_h_
#define __RichBaseObject_h_

namespace gui
{

class RichWarnWidget_ABC;

// =============================================================================
/** @class  RichBaseObject
    @brief  RichBaseObject
*/
// Created: ABR 2013-06-07
// =============================================================================
class RichBaseObject : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichBaseObject( RichWarnWidget_ABC& widget, QObject* parent = 0 );
    virtual ~RichBaseObject();
    //@}

public slots:
    //! @name Abstract method
    //@{
    void OnBlink();
    void OnDone();
    //@}

private:
    RichWarnWidget_ABC& widget_;
};

} //! namespace gui

#endif // __RichBaseObject_h_
