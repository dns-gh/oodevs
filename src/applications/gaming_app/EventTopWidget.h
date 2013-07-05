// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTopWidget_h_
#define __EventTopWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventTopWidget
    @brief  EventTopWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTopWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventTopWidget();
    virtual ~EventTopWidget();
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
    QLabel* title_;
    QLabel* source_;
    //@}
};

#endif // __EventTopWidget_h_
