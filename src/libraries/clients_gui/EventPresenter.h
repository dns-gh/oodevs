// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventPresenter_h_
#define __EventPresenter_h_

#include "EventSubPresenter_ABC.h"
#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#pragma warning( push )
#pragma warning( disable : 4251 )
#include <QtCore/qobject.h>
#include <QtGui/qcolor.h>
#pragma warning( pop )

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class MissionInterface_ABC;
    }
}

namespace kernel
{
    class AgentTypes;
}

class QDateTime;

namespace gui
{
    class Event;
    class EventFactory;
    class EventOrderPresenter;
    class EventOrderView_ABC;
    class EventView_ABC;
    struct EventViewState;
    class TimelineHandler_ABC;

// =============================================================================
/** @class  EventPresenter
    @brief  EventPresenter
*/
// Created: ABR 2013-11-19
// =============================================================================
class EventPresenter : public QObject
{
    Q_OBJECT

    //! @name Types
    //@{
    typedef std::map< E_EventTypes, EventSubPresenter_ABC::T_SharedPtr > T_Presenters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             EventPresenter( EventView_ABC& view,
                             const EventFactory& factory );
    virtual ~EventPresenter();
    //@}

    //! @name Specialized presenters
    //@{
    void AddSubPresenter( E_EventTypes type,
                          const EventSubPresenter_ABC::T_SharedPtr& presenter );
    EventSubPresenter_ABC& GetCurrentPresenter() const;
    EventSubPresenter_ABC& GetPresenter( E_EventTypes type ) const;
    bool HasCurrentPresenter() const;
    //@}

    //! @name Operations
    //@{
    void Purge();
    void SetTimelineHandler( const boost::shared_ptr< TimelineHandler_ABC >& timelineHandler );
    void OnWarningChanged( const std::string& warning, const QColor& warningColor );
    //@}

public slots:
    //! @name Slots
    //@{
    void StartCreation( E_EventTypes type,
                        const QDateTime& beginDate );
    void StartEdition( const gui::Event& event,
                       bool raise,
                       bool purge = true );

    void OnEventContentChanged();
    void OnEventUpdated( const gui::Event& event );
    void OnEventDeleted( const gui::Event& event );

    void OnBeginDateChanged( const QDateTime& begin );
    void OnEndDateActivated( bool activated );
    void OnEndDateChanged( const QDateTime& end );

    void OnTriggerClicked();
    void OnClearClicked();
    void OnDetailClicked();
    void OnSaveClicked();
    void OnSaveAsClicked();
    //@}

private:
    //! @name Helpers
    //@{
    void CheckEvent() const;
    bool IsCurrentEvent( const gui::Event& event );
    void InternalPurge();

    void BuildView( E_EventDockModes mode,
                    bool raise = false,
                    const QColor& warningColor = Qt::transparent,
                    const std::string& warning = "" );
    void UpdateView();
    void UpdateViewAfterEdition();
    void ChangeMode( E_EventDockModes mode,
                     bool raise,
                     bool detail,
                     const QColor& warningColor,
                     const std::string& warning );

    void Delete();        // delete current event
    void Select();        // select event in the timeline web view
    void Trigger();       // send event to the simulation
    void Plan();          // send event to the timeline server
    void UpdateRemote();  // update event in the timeline server
    void UpdateCurrent(); // update current event with view data
    //@}

private:
    //! @name Member data
    //@{
    EventView_ABC& view_;
    const EventFactory& factory_;
    boost::scoped_ptr< EventViewState > state_;
    boost::shared_ptr< Event > event_;
    boost::shared_ptr< TimelineHandler_ABC > timelineHandler_;
    T_Presenters presenters_;
    //@}
};

} //! namespace gui

#endif // __EventPresenter_h_
