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
#include "clients_gui/Event.h"
#include "clients_gui/EventTaskPresenter.h"
#include "clients_gui/EventTaskViewState.h"

namespace gui
{
    std::ostream& operator<<( std::ostream& os, const gui::EventTaskViewState& state )
    {
        os << std::endl << "EventTaskViewState " << "{ "
           << state.label_ << ", "
           << state.description_ << ", "
           << state.url_ << ", "
           << state.payload_ << ", "
           << state.isUrlValid_ << ", "
           << state.isPayloadVisible_ << ", "
           << state.bytes_ << " }" << std::endl;
        return os;
    }
}

namespace
{
    bool operator==( const gui::EventTaskViewState& lhs, const gui::EventTaskViewState& rhs )
    {
        return lhs.label_ == rhs.label_ &&
               lhs.description_ == rhs.description_ &&
               lhs.url_ == rhs.url_ &&
               lhs.payload_ == rhs.payload_ &&
               lhs.isUrlValid_ == rhs.isUrlValid_ &&
               lhs.isPayloadVisible_ == rhs.isPayloadVisible_ &&
               lhs.bytes_ == rhs.bytes_;
    }

    struct TaskPresenterFixture : public PresenterBaseFixture
    {
        TaskPresenterFixture()
            : taskPresenter( taskView, controllers, entityResolver )
            , state( 0, "label", "description", "url", "payload", true, false, 7 )
        {
            taskEvent.reset( eventFactory.Create( eEventTypes_Task ) ) ;
            taskEvent->GetEvent().name = "label";
            taskEvent->GetEvent().info = "description";
            taskEvent->GetEvent().action.target = "url";
            taskEvent->GetEvent().action.payload = "cGF5bG9hZA==";
        }

        MockEventView< gui::EventTaskViewState > taskView;
        gui::EventTaskPresenter taskPresenter;
        gui::EventTaskViewState state;
        boost::shared_ptr< gui::Event > taskEvent;

        void InitializePresenter()
        {
            taskPresenter.FillFrom( *taskEvent );
        }
        void CheckBuild()
        {
            MOCK_EXPECT( taskView.BlockSignals ).once().with( true );
            MOCK_EXPECT( taskView.Build ).once().with( state );
            MOCK_EXPECT( taskView.BlockSignals ).once().with( false );
        }
    };

} //! anonymous namespace

BOOST_FIXTURE_TEST_CASE( task_presenter_on_change_label, TaskPresenterFixture )
{
    InitializePresenter();

    // change label
    state.label_ = "new label";
    CheckBuild();
    taskPresenter.OnLabelChanged( "new label" );
}

BOOST_FIXTURE_TEST_CASE( task_presenter_on_change_description, TaskPresenterFixture )
{
    InitializePresenter();

    // change description
    state.description_ = "new description";
    CheckBuild();
    taskPresenter.OnDescriptionChanged( "new description" );
}

BOOST_FIXTURE_TEST_CASE( task_presenter_on_change_url, TaskPresenterFixture )
{
    InitializePresenter();

    // change url
    state.url_ = "new url";
    CheckBuild();
    taskPresenter.OnUrlChanged( "new url" );

    // change url to an invalid one
    state.url_ = "";
    state.isUrlValid_ = false;
    CheckBuild();
    taskPresenter.OnUrlChanged( "" );

    // change url to a valid one
    state.url_ = "url";
    state.isUrlValid_ = true;
    CheckBuild();
    taskPresenter.OnUrlChanged( "url" );
}

BOOST_FIXTURE_TEST_CASE( task_presenter_on_change_payload, TaskPresenterFixture )
{
    InitializePresenter();

    // change payload
    state.payload_ = "new payload";
    CheckBuild();
    taskPresenter.OnPayloadChanged( "new payload" );
}

BOOST_FIXTURE_TEST_CASE( task_presenter_on_show_clicked, TaskPresenterFixture )
{
    InitializePresenter();

    // click on show
    state.isPayloadVisible_ = true;
    CheckBuild();
    taskPresenter.OnShowClicked();

    // click on show
    state.isPayloadVisible_ = false;
    CheckBuild();
    taskPresenter.OnShowClicked();
}
