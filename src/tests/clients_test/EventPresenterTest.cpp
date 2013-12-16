// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "MockEventTools.h"

#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventView_ABC.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/TimelineHandler_ABC.h"

#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

namespace gui
{
    std::ostream& operator<<( std::ostream& os, const kernel::Event& event )
    {
        os << "Event " << "{ "
           << ENT_Tr::ConvertFromEventType( event.GetType() ) << ", "
           << event.GetEvent().uuid << ", "
           << event.GetEvent().name << ", "
           << event.GetEvent().info << ", "
           << event.GetEvent().begin << ", "
           << event.GetEvent().end << ", "
           << event.GetEvent().done << ", "
           << event.GetEvent().action.target << ", "
           << event.GetEvent().action.apply << ", "
           << event.GetEvent().action.payload << " }";
        return os;
    }
    std::ostream& operator<<( std::ostream& os, const gui::EventViewState& state )
    {
        os << std::endl << "EventViewState " << "{ ";
        if( state.event_ )
            os << *state.event_ << ", ";
        else
            os << "<empty event>" << ", ";
        os << state.mode_ << ", "
           << state.trigger_ << ", "
           << state.save_ << ", "
           << state.saveAs_ << ", "
           << state.clear_ << ", "
           << state.raise_ << ", "
           << state.detail_ << ", "
           << state.bottomToolBar_ << ", "
           << state.warningColor_.name().toStdString() << ", "
           << state.warning_ << " }" << std::endl;
        return os;
    }
}

namespace
{
    bool operator==( const timeline::Event& lhs, const timeline::Event& rhs )
    {
        return lhs.uuid == rhs.uuid &&
               lhs.name == rhs.name &&
               lhs.info == rhs.info &&
               lhs.begin == rhs.begin &&
               lhs.end == rhs.end &&
               lhs.done == rhs.done &&
               lhs.action.target == rhs.action.target &&
               lhs.action.apply  == rhs.action.apply  &&
               lhs.action.payload == rhs.action.payload;
    }
    bool operator==( const kernel::Event& lhs, const kernel::Event& rhs )
    {
        return lhs.GetType() == rhs.GetType() &&
               lhs.GetEvent() == rhs.GetEvent();
    }
    bool operator==( const gui::EventViewState& lhs, const gui::EventViewState& rhs )
    {
        if( lhs.event_ && !rhs.event_ ||
            !lhs.event_ && rhs.event_ )
            return false;
        bool equalEvents = ( lhs.event_ ) ? *lhs.event_ == *rhs.event_ : true;
        return equalEvents &&
               lhs.mode_ == rhs.mode_ &&
               lhs.trigger_ == rhs.trigger_ &&
               lhs.save_ == rhs.save_ &&
               lhs.saveAs_ == rhs.saveAs_ &&
               lhs.clear_ == rhs.clear_ &&
               lhs.raise_ == rhs.raise_ &&
               lhs.detail_ == rhs.detail_ &&
               lhs.bottomToolBar_ == rhs.bottomToolBar_ &&
               lhs.warningColor_ == rhs.warningColor_ &&
               lhs.warning_ == rhs.warning_;
    }

    MOCK_BASE_CLASS( MockEventView, gui::EventView_ABC )
    {
        MOCK_METHOD( Purge, 0 );
        MOCK_METHOD( Build, 1 );
        MOCK_METHOD( Update, 1 );
        MOCK_METHOD( BlockSignals, 1 );
        MOCK_METHOD( Draw, 1 );
    };
    MOCK_BASE_CLASS( MockTimelineHandler, gui::TimelineHandler_ABC )
    {
        MOCK_METHOD( CreateEvent, 1 );
        MOCK_METHOD( SelectEvent, 1 );
        MOCK_METHOD( EditEvent, 1 );
        MOCK_METHOD( DeleteEvent, 1 );
    };
    MOCK_BASE_CLASS( MockSubPresenter, gui::EventSubPresenter_ABC )
    {
        MOCK_METHOD( ShouldEnableTrigger, 0 );
        MOCK_METHOD( ShouldEnableClear, 0 );
        MOCK_METHOD( Trigger, 0 );
        MOCK_METHOD( CommitTo, 1 );
        MOCK_METHOD( Purge, 0 );
        MOCK_METHOD( Clear, 0 );
    };

    const QDateTime first_date = QDateTime::fromString( "2013-12-06T19:20:30", Qt::ISODate );
    const QDateTime second_date = QDateTime::fromString( "2013-13-07T20:30:40", Qt::ISODate );
    const QDateTime third_date = QDateTime::fromString( "2013-14-08T21:40:50", Qt::ISODate );
    const std::string warningErrorText = "warning text";
    const std::string warningInfoText = "Read only";
    const QColor warningErrorColor = Qt::red;
    const QColor warningInfoColor = Qt::darkGreen;

    struct PresenterFixture : public PresenterBaseFixture
    {
        PresenterFixture()
            : presenter( view, eventFactory )
        {
            orderPresenter = boost::make_shared< MockSubPresenter >();
            taskPresenter = boost::make_shared< MockSubPresenter >();
            presenter.AddSubPresenter( eEventTypes_Order, orderPresenter );
            presenter.AddSubPresenter( eEventTypes_Task, taskPresenter );

            orderEvent.reset( eventFactory.Create( eEventTypes_Order ) ) ;
            orderEvent->GetEvent().begin = first_date.toString( EVENT_DATE_FORMAT );

            taskEvent.reset( eventFactory.Create( eEventTypes_Task ) ) ;
            taskEvent->GetEvent().begin = first_date.toString( EVENT_DATE_FORMAT );

            timelineHandler = boost::make_shared< MockTimelineHandler >();
            presenter.SetTimelineHandler( timelineHandler );
        }

        MockEventView view;
        gui::EventPresenter presenter;
        gui::EventViewState state;
        boost::shared_ptr< MockTimelineHandler > timelineHandler;
        boost::shared_ptr< MockSubPresenter > orderPresenter;
        boost::shared_ptr< MockSubPresenter > taskPresenter;
        boost::shared_ptr< kernel::Event > orderEvent;
        boost::shared_ptr< kernel::Event > taskEvent;

        void CheckPurge()
        {
            MOCK_EXPECT( view.Purge ).once();
            MOCK_EXPECT( orderPresenter->Purge ).once();
            MOCK_EXPECT( taskPresenter->Purge ).once();
        }
        void CheckPurgeAndInit()
        {
            state = gui::EventViewState();
            CheckPurge();
            MOCK_EXPECT( view.Build ).once().with( state );
            MOCK_EXPECT( view.Update ).once().with( state );
        }
        void CheckUpdate( const boost::shared_ptr< MockSubPresenter >& subPresenter )
        {
            MOCK_EXPECT( subPresenter->ShouldEnableTrigger ).once().returns( false );
            MOCK_EXPECT( subPresenter->ShouldEnableClear ).once().returns( false );
            MOCK_EXPECT( view.Update ).once().with( state );
        }
        void CheckBuild( const boost::shared_ptr< MockSubPresenter >& subPresenter )
        {
            MOCK_EXPECT( view.Build ).once().with( state );
            CheckUpdate( subPresenter );
        }
        void CheckPurgeAndBuild( const boost::shared_ptr< MockSubPresenter >& subPresenter )
        {
            CheckPurge();
            CheckBuild( subPresenter );
        }
    };

} // anonymous namespace

BOOST_FIXTURE_TEST_CASE( event_presenter_initialization, PresenterFixture )
{
    CheckPurgeAndInit();
    presenter.Purge();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_start_creation, PresenterFixture )
{
    // Create an event type
    state = gui::EventViewState( orderEvent, eEventDockModes_Create, false, true, false, false, true, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartCreation( eEventTypes_Order, first_date );

    // Create another type
    state = gui::EventViewState( taskEvent, eEventDockModes_Create, false, true, false, false, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartCreation( eEventTypes_Task, first_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_start_edition, PresenterFixture )
{
    // Edit 1 event
    state = gui::EventViewState( orderEvent, eEventDockModes_EditPlanned, false, true, true, false, false, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartEdition( *orderEvent, false );

    // Edit with a raise request
    state.raise_ = true;
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartEdition( *orderEvent, true );

    // Edit another type
    state.event_ = taskEvent;
    state.bottomToolBar_ = false;
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    // Edit a triggered event
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_warning_changed_on_planned_event, PresenterFixture )
{
    state = gui::EventViewState( orderEvent, eEventDockModes_Create, false, true, false, false, true, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartCreation( eEventTypes_Order, first_date );

    state.warning_ = warningErrorText;
    state.warningColor_ = warningErrorColor;
    CheckUpdate( orderPresenter );
    presenter.OnWarningChanged( warningErrorText, warningErrorColor );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_warning_changed_on_triggered_event, PresenterFixture )
{
    orderEvent->GetEvent().done = true;
    state = gui::EventViewState( orderEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, true, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartEdition( *orderEvent, true );

    state.warning_ = warningErrorText;
    state.warningColor_ = warningErrorColor;
    CheckUpdate( orderPresenter );
    presenter.OnWarningChanged( warningErrorText, warningErrorColor );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_event_content_changed_on_planned_event, PresenterFixture )
{
    state = gui::EventViewState( orderEvent, eEventDockModes_Create, false, true, false, false, true, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartCreation( eEventTypes_Order, first_date );

    state.raise_ = false;
    MOCK_EXPECT( orderPresenter->CommitTo ).once().with( orderEvent->GetEvent() );
    CheckUpdate( orderPresenter );
    presenter.OnEventContentChanged();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_event_content_changed_on_triggered_event, PresenterFixture )
{
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    state = gui::EventViewState( taskEvent, eEventDockModes_EditTriggered, false, false, true );
    MOCK_EXPECT( taskPresenter->CommitTo ).once().with( taskEvent->GetEvent() );
    CheckUpdate( taskPresenter );
    presenter.OnEventContentChanged();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_event_updated, PresenterFixture )
{
    orderEvent->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    taskEvent->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );

    // Edit an existing event
    state = gui::EventViewState( orderEvent, eEventDockModes_EditPlanned, false, true, true, false, false, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartEdition( *orderEvent, false );

    // Update another one, nothing happen
    presenter.OnEventUpdated( *taskEvent );

    // Update current event, re-build
    CheckPurgeAndBuild( orderPresenter );
    presenter.OnEventUpdated( *orderEvent );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_event_deleted, PresenterFixture )
{
    orderEvent->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    taskEvent->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );

    // Edit an existing event
    state = gui::EventViewState( orderEvent, eEventDockModes_EditPlanned, false, true, true, false, false, false, true );
    CheckPurgeAndBuild( orderPresenter );
    presenter.StartEdition( *orderEvent, false );

    // Delete another on, nothing happen
    presenter.OnEventDeleted( *taskEvent );

    // Delete current event, build with a copy of deleted event (used as a triggered event, so we can't save it)
    state = gui::EventViewState( orderEvent, eEventDockModes_EditTriggered, false, false, true, false, false, false, true );
    CheckBuild( orderPresenter );
    presenter.OnEventDeleted( *orderEvent );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_begin_date_changed_on_planned_event, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.event_->GetEvent().begin = second_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnBeginDateChanged( second_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_begin_date_changed_on_triggered_event, PresenterFixture )
{
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    state = gui::EventViewState( taskEvent, eEventDockModes_EditTriggered, false, false, true );
    state.event_->GetEvent().begin = second_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnBeginDateChanged( second_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_end_date_activated_on_planned_event, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.event_->GetEvent().end = state.event_->GetEvent().begin;
    CheckUpdate( taskPresenter );
    presenter.OnEndDateActivated( true );

    state.event_->GetEvent().end.clear();
    CheckUpdate( taskPresenter );
    presenter.OnEndDateActivated( false );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_end_date_activated_on_triggered_event, PresenterFixture )
{
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    state = gui::EventViewState( taskEvent, eEventDockModes_EditTriggered, false, false, true );
    state.event_->GetEvent().end = state.event_->GetEvent().begin;
    CheckUpdate( taskPresenter );
    presenter.OnEndDateActivated( true );

    state.event_->GetEvent().end.clear();
    CheckUpdate( taskPresenter );
    presenter.OnEndDateActivated( false );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_end_date_changed_on_planned_event, PresenterFixture )
{
    // End date of a new/plan event changed, keep current mode
    taskEvent->GetEvent().end = second_date.toString( EVENT_DATE_FORMAT );
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.event_->GetEvent().end = third_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnEndDateChanged( third_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_end_date_changed_on_triggered_event, PresenterFixture )
{
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    state = gui::EventViewState( taskEvent, eEventDockModes_EditTriggered, false, false, true );
    state.event_->GetEvent().end = third_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnEndDateChanged( third_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_trigger_clicked_on_planned_event, PresenterFixture )
{
    taskEvent->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    MOCK_EXPECT( timelineHandler->DeleteEvent ).once().with( taskEvent->GetEvent().uuid );
    MOCK_EXPECT( taskPresenter->Trigger ).once();
    presenter.OnTriggerClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_trigger_clicked_on_new_event, PresenterFixture )
{
    taskEvent->GetEvent().uuid.clear();
    state = gui::EventViewState( taskEvent, eEventDockModes_Create, false, true, false, false, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartCreation( eEventTypes_Task, first_date );

    MOCK_EXPECT( taskPresenter->Trigger ).once();
    presenter.OnTriggerClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_clear_clicked, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    MOCK_EXPECT( taskPresenter->Clear ).once();
    presenter.OnClearClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_detail_clicked, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.detail_ = true;
    CheckUpdate( taskPresenter );
    presenter.OnDetailClicked();

    state.detail_ = false;
    CheckUpdate( taskPresenter );
    presenter.OnDetailClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_start_edition_remove_details, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.detail_ = true;
    CheckUpdate( taskPresenter );
    presenter.OnDetailClicked();

    state.detail_ = false;
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_updating_content_remove_details, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    state.detail_ = true;
    CheckUpdate( taskPresenter );
    presenter.OnDetailClicked();

    state.detail_ = false;
    state.event_->GetEvent().begin = second_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnBeginDateChanged( second_date );
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_save_clicked_on_planned_event, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    MOCK_EXPECT( taskPresenter->CommitTo ).once().with( taskEvent->GetEvent() );
    MOCK_EXPECT( timelineHandler->EditEvent ).once().with( taskEvent->GetEvent() );
    CheckBuild( taskPresenter );
    presenter.OnSaveClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_save_clicked_on_new_event, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_Create, false, true, false, false, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartCreation( eEventTypes_Task, first_date );

    // can't know the generated uuid here, so can't expect with( state )
    MOCK_EXPECT( timelineHandler->CreateEvent ).once();
    MOCK_EXPECT( view.Build ).once();
    MOCK_EXPECT( taskPresenter->ShouldEnableTrigger ).once().returns( false );
    MOCK_EXPECT( taskPresenter->ShouldEnableClear ).once().returns( false );
    MOCK_EXPECT( view.Update ).once();
    presenter.OnSaveClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_on_save_as_clicked, PresenterFixture )
{
    state = gui::EventViewState( taskEvent, eEventDockModes_EditPlanned, false, true, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, false );

    // can't know the generated uuid here, so can't expect with( state )
    MOCK_EXPECT( timelineHandler->CreateEvent ).once();
    MOCK_EXPECT( view.Build ).once();
    MOCK_EXPECT( taskPresenter->ShouldEnableTrigger ).once().returns( false );
    MOCK_EXPECT( taskPresenter->ShouldEnableClear ).once().returns( false );
    MOCK_EXPECT( view.Update ).once();
    presenter.OnSaveAsClicked();
}

BOOST_FIXTURE_TEST_CASE( event_presenter_throw_if_a_forbidden_action_is_triggered , PresenterFixture )
{
    // eEventDockModes_None
    CheckPurgeAndInit();
    presenter.Purge();

    BOOST_CHECK_THROW( presenter.OnTriggerClicked(), tools::Exception );
    BOOST_CHECK_THROW( presenter.OnSaveClicked(), tools::Exception );
    BOOST_CHECK_THROW( presenter.OnSaveAsClicked(), tools::Exception );

    // eEventDockModes_DisplayTriggered
    taskEvent->GetEvent().done = true;
    state = gui::EventViewState( taskEvent, eEventDockModes_DisplayTriggered, false, false, true, false, true, false, false, warningInfoColor, warningInfoText );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartEdition( *taskEvent, true );

    BOOST_CHECK_THROW( presenter.OnSaveClicked(), tools::Exception );

    // eEventDockModes_EditTriggered
    state = gui::EventViewState( taskEvent, eEventDockModes_EditTriggered, false, false, true );
    state.event_->GetEvent().begin = second_date.toString( EVENT_DATE_FORMAT );
    CheckUpdate( taskPresenter );
    presenter.OnBeginDateChanged( second_date );

    BOOST_CHECK_THROW( presenter.OnSaveClicked(), tools::Exception );

    // eEventDockModes_Create
    taskEvent->GetEvent().done = false;
    taskEvent->GetEvent().begin = first_date.toString( EVENT_DATE_FORMAT );
    state = gui::EventViewState( taskEvent, eEventDockModes_Create, false, true, false, false, true );
    CheckPurgeAndBuild( taskPresenter );
    presenter.StartCreation( eEventTypes_Task, first_date );

    BOOST_CHECK_THROW( presenter.OnSaveAsClicked(), tools::Exception );
}
