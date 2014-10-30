// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_test_pch.h"

#include "MockEventTools.h"
#include "clients_gui/Event.h"
#include "clients_gui/EventMarkerPresenter.h"
#include "clients_gui/EventMarkerViewState.h"
#include <boost/property_tree/ptree.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace bpt = boost::property_tree;

namespace gui
{
    std::ostream& operator<<( std::ostream& os, const gui::EventMarkerViewState& state )
    {
        os << std::endl << "EventMarkerViewState " << "{ "
           << state.label_ << ", "
           << state.description_ << ", "
           << state.resetDrawings_ << ", "
           << state.drawingsPath_ << ", "
           << state.configurationPath_
           << " }" << std::endl;
        return os;
    }
}

namespace
{
    bool operator==( const gui::EventMarkerViewState& lhs, const gui::EventMarkerViewState& rhs )
    {
        return lhs.label_ == rhs.label_ &&
            lhs.description_ == rhs.description_&&
            lhs.resetDrawings_ == rhs.resetDrawings_&&
            lhs.drawingsPath_ == rhs.drawingsPath_&&
            lhs.configurationPath_ == rhs.configurationPath_;
    }

    struct MarkerPresenterFixture : public PresenterBaseFixture
    {
        MarkerPresenterFixture()
            : markerPresenter( markerView )
            , state( "label", "description", false, "c:\\path1\\drawings.xml", "d:\\path2\\configuration.xml" )
        {
            markerEvent.reset( eventFactory.Create( eEventTypes_Marker ) );
            markerEvent->GetEvent().name = "label";
            markerEvent->GetEvent().info = "description";
            bpt::ptree rpy;
            rpy.put( "reset_drawings", false );
            rpy.put( "drawings_path", "c:\\path1\\drawings.xml" );
            rpy.put( "configuration_path", "d:\\path2\\configuration.xml" );
            std::ostringstream output;
            bpt::write_json( output, rpy );
            markerEvent->GetEvent().action.payload = output.str();
        }

        MockEventView< gui::EventMarkerViewState > markerView;
        gui::EventMarkerPresenter markerPresenter;
        gui::EventMarkerViewState state;
        boost::shared_ptr< gui::Event > markerEvent;

        void InitializePresenter()
        {
            markerPresenter.FillFrom( *markerEvent );
        }
        void CheckBuild()
        {
            MOCK_EXPECT( markerView.BlockSignals ).once().with( true );
            MOCK_EXPECT( markerView.Build ).once().with( state );
            MOCK_EXPECT( markerView.BlockSignals ).once().with( false );
        }
    };

} //! anonymous namespace

BOOST_FIXTURE_TEST_CASE( marker_presenter_on_change_label, MarkerPresenterFixture )
{
    InitializePresenter();

    // change label
    state.label_ = "new label";
    CheckBuild();
    markerPresenter.OnLabelChanged( "new label" );
}

BOOST_FIXTURE_TEST_CASE( marker_presenter_on_change_description, MarkerPresenterFixture )
{
    InitializePresenter();

    // change description
    state.description_ = "new description";
    CheckBuild();
    markerPresenter.OnDescriptionChanged( "new description" );
}

BOOST_FIXTURE_TEST_CASE( marker_presenter_on_change_reset_drawings, MarkerPresenterFixture )
{
    InitializePresenter();

    // change reset drawings
    state.resetDrawings_ = true;
    CheckBuild();
    markerPresenter.OnResetDrawingsChanged( true );
}

BOOST_FIXTURE_TEST_CASE( marker_presenter_on_change_drawings_path, MarkerPresenterFixture )
{
    InitializePresenter();

    // change drawings path
    state.drawingsPath_ = tools::Path::FromUTF8( "d:\\path3\\drawings2.xml" );
    CheckBuild();
    markerPresenter.OnDrawingsPathChanged( tools::Path::FromUTF8( "d:\\path3\\drawings2.xml" ) );
}

BOOST_FIXTURE_TEST_CASE( marker_presenter_on_change_configuration_path, MarkerPresenterFixture )
{
    InitializePresenter();

    // change configuration path
    state.configurationPath_ = tools::Path::FromUTF8( "c:\\path4\\configuration2.xml" );
    CheckBuild();
    markerPresenter.OnConfigurationPathChanged( tools::Path::FromUTF8( "c:\\path4\\configuration2.xml" ) );
}
