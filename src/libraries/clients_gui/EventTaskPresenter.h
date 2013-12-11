// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskPresenter_h_
#define __EventTaskPresenter_h_

#include "EventSubPresenter_ABC.h"
#include <boost/scoped_ptr.hpp>

namespace gui
{
    class EventTaskView_ABC;
    struct EventTaskViewState;

// =============================================================================
/** @class  EventTaskPresenter
    @brief  EventTaskPresenter
*/
// Created: ABR 2013-12-09
// =============================================================================
class EventTaskPresenter : public QObject
                         , public EventSubPresenter_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskPresenter( EventTaskView_ABC& view );
    virtual ~EventTaskPresenter();
    //@}

    //! @name Operations
    //@{
    void FillFromEvent( timeline::Event& event );
    void OnDescriptionChanged( const QString& label );
    void OnPayloadChanged( const QString& label );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLabelChanged( const QString& label );
    void OnUrlChanged( const QString& label );
    void OnShowClicked();
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual void Trigger();
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Purge();
    virtual void Clear();
    //@}

    //! @name Helper
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    EventTaskView_ABC& view_;
    boost::scoped_ptr< EventTaskViewState > state_;
    //@}
};

} //! namespace gui

#endif // __EventTaskPresenter_h_
