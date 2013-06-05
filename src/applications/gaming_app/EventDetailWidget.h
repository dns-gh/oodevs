// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventDetailWidget_h_
#define __EventDetailWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventDetailWidget
    @brief  EventDetailWidget
*/
// Created: ABR 2013-05-31
// =============================================================================
class EventDetailWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventDetailWidget();
    virtual ~EventDetailWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* uuid_;
    QLineEdit* name_;
    QTextEdit* info_;
    QLineEdit* begin_;
    QLineEdit* end_;
    QCheckBox* done_;
    QLineEdit* target_;
    QCheckBox* apply_;
    QTextEdit* payload_;
    //@}
};

#endif // __EventDetailWidget_h_
