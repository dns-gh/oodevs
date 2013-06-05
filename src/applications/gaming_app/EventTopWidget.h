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

namespace kernel
{
    class Time_ABC;
}

// =============================================================================
/** @class  EventTopWidget
    @brief  EventTopWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTopWidget : public EventWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTopWidget( const kernel::Time_ABC& simulation );
    virtual ~EventTopWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBeginDateTimeChanged( const QDateTime& dateTime );
    void OnHasEndTimeChanged( int state );
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
    const kernel::Time_ABC& simulation_;

    QLabel* title_;
    QLabel* source_;

    QDateTimeEdit* beginDateTimeEdit_;
    QCheckBox* hasEndDateTimeCheckbox_;
    QDateTimeEdit* endDateTimeEdit_;
    //@}
};

#endif // __EventTopWidget_h_
