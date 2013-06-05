// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventBottomWidget_h_
#define __EventBottomWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventBottomWidget
    @brief  EventBottomWidget
*/
// Created: ABR 2013-05-30
// =============================================================================
class EventBottomWidget : public EventWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventBottomWidget();
    virtual ~EventBottomWidget();
    //@}

    //! @name Operations
    //@{
    //@}

signals:
    //! @name Signals
    //@{
    void Trigger();
    void CreateReminder();
    void ShowDetail();
    void Discard();
    void Save();
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
    QAction* flagAsCompleteAction_;
    QAction* detailAction_;
    //@}
};

#endif // __EventBottomWidget_h_
