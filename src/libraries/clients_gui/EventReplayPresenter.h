// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventReplayPresenter_h_
#define __EventReplayPresenter_h_

#include "EventSubPresenter_ABC.h"

namespace gui
{
    struct EventReplayViewState;
    template< typename T > class EventView_ABC;

// =============================================================================
/** @class  EventReplayPresenter
    @brief  EventReplayPresenter
*/
// Created: JSR 2014-10-24
// =============================================================================
class EventReplayPresenter : public QObject
                           , public EventSubPresenter_ABC< EventReplayViewState >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit EventReplayPresenter( EventView_ABC< EventReplayViewState >& view );
    virtual ~EventReplayPresenter();
    //@}

public:
    //! @name Operations
    //@{
    virtual void FillFrom( const Event& event );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLabelChanged( const QString& label );
    void OnDescriptionChanged( const QString& description );
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual void Trigger( const Event& event );
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Clear();
    //@}
};

} //! namespace gui

#endif // __EventReplayPresenter_h_
