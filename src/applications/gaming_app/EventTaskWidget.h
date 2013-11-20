// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskWidget_h_
#define __EventTaskWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventTaskWidget
    @brief  EventTaskWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTaskWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskWidget();
    virtual ~EventTaskWidget();
    //@}

    //! @name Operations
    //@{
    //@}
private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event );
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* label_;
    QTextEdit* description_;
    QComboBox* profiles_;
    //@}
};

#endif // __EventTaskWidget_h_
