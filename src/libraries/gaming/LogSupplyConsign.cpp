//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogSupplyConsign.h"
#include "LogisticConsigns.h"
#include "Simulation.h"
#include "LogConsignDisplayer_ABC.h"
#include "SupplyRecipientResourcesRequest.h"
#include "SupplyResourceRequest.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::LogSupplyConsign( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver
                                  , const tools::Resolver_ABC< Agent_ABC >& agentResolver
                                  , const tools::Resolver_ABC< Formation_ABC >&   formationResolver
                                  , const tools::Resolver_ABC< DotationType >& dotationResolver
                                  , const Simulation& simulation
                                  , const sword::LogSupplyHandlingCreation& message )
    : controller_                        ( controller )
    , resolver_                          ( resolver )
    , agentResolver_                     ( agentResolver )
    , formationResolver_                 ( formationResolver )
    , dotationResolver_                  ( dotationResolver )
    , nID_                               ( message.request().id() )
    , pLogHandlingEntity_                ( controller_, FindLogEntity( message.supplier() ) )
    , pPionLogConvoying_                 ( controller_ )
    , pLogProvidingConvoyResourcesEntity_( controller_, FindLogEntity( message.transporters_provider() ) )
    , nState_                            ( eLogSupplyHandlingStatus_Termine )
    , currentStateEndTick_               ( std::numeric_limits< unsigned int >::max() )
    , simulation_                        ( simulation )
{
    if( pLogHandlingEntity_ )
        pLogHandlingEntity_.ConstCast()->Get< LogSupplyConsigns >().HandleConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogSupplyConsign::~LogSupplyConsign()
{
    for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
        it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *this );
    if( pLogHandlingEntity_ )
        pLogHandlingEntity_.ConstCast()->Get< LogSupplyConsigns >().TerminateConsign( *this );
    if( pPionLogConvoying_ )
        pPionLogConvoying_.ConstCast()->Get< LogSupplyConsigns >().TerminateConsign( *this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::OnReceiveMsgUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Update( const sword::LogSupplyHandlingUpdate& message )
{
    if( message.has_convoyer() && ( !pPionLogConvoying_ || message.convoyer().id() != int( pPionLogConvoying_->GetId() ) ) )
    {
        if( pPionLogConvoying_ )
            pPionLogConvoying_.ConstCast()->Get< LogSupplyConsigns >().TerminateConsign( *this );
        pPionLogConvoying_ = agentResolver_.Find( message.convoyer().id() );
        if( pPionLogConvoying_ )
            pPionLogConvoying_.ConstCast()->Get< LogSupplyConsigns >().HandleConsign( *this );
    }
    if( message.has_state()  )
        nState_ = E_LogSupplyHandlingStatus( message.state() );
    if( message.has_current_state_end_tick() )
        currentStateEndTick_ = message.current_state_end_tick();
    if( message.has_requests() )
    {
        for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
            it.NextElement().recipient_.Get< LogSupplyConsigns >().RemoveConsign( *this );
        DeleteAll();
        BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, message.requests().requests() )
        {
            SupplyRecipientResourcesRequest* tmp = new SupplyRecipientResourcesRequest( dotationResolver_, resolver_, data );
            Register( data.recipient().id(), *tmp );
            tmp->recipient_.Get< LogSupplyConsigns >().AddConsign( *this );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogSupplyConsign::Display( LogConsignDisplayer_ABC& displayer, kernel::DisplayExtractor_ABC& displayExtractor ) const
{
    gui::DisplayExtractor& extractor = *static_cast< gui::DisplayExtractor* >( &displayExtractor );

    unsigned nbRequests = Count();
    if( nbRequests == 0 )
        displayer.DisplayTitle( tools::translate( "Logistic", "No recipients" ), tools::ToString( nState_ ) );
    else if( nbRequests == 1 )
        displayer.DisplayTitle( CreateIterator().NextElement().recipient_.GetTooltip(), tools::ToString( nState_ ) );
    else
        displayer.DisplayTitle( tools::translate( "Logistic", "Multiple recipients" ), tools::ToString( nState_ ) );

    tools::Iterator< const SupplyRecipientResourcesRequest& > itRecipient = CreateIterator();
    while( itRecipient.HasMoreElements() )
    {
        QMap< QString, LogConsignDisplayer_ABC::T_OrderedValues > requests;
        const SupplyRecipientResourcesRequest& curRecipient= itRecipient.NextElement();
        tools::Iterator< const SupplyResourceRequest& > itRequest = curRecipient.CreateIterator();
        while( itRequest.HasMoreElements() )
        {
            const SupplyResourceRequest& curRequest = itRequest.NextElement();
            LogConsignDisplayer_ABC::T_OrderedValues& curRequestQuantities = requests[ curRequest.GetTypeName() ];
            curRequestQuantities.reserve( 3 );
            curRequestQuantities.push_back( std::make_pair( tools::translate( "Logistic", "requested" ), curRequest.GetRequested() ) );
            curRequestQuantities.push_back( std::make_pair( tools::translate( "Logistic", "granted" ), curRequest.GetGranted() ) );
            curRequestQuantities.push_back( std::make_pair( tools::translate( "Logistic", "convoyed" ), curRequest.GetConvoyed() ) );
        }
        displayer.DisplaySubItemValues( tools::translate( "Logistic", "Recipients" ),
                                        curRecipient.GetRecipientTooltip(),
                                        requests );
    }

    if( pPionLogConvoying_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Convoyer:" ), extractor.GetDisplayName( *pPionLogConvoying_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "Instruction:" ), extractor.GetDisplayName( nID_ ) );
    if( pLogHandlingEntity_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Supplier:" ), extractor.GetDisplayName( *pLogHandlingEntity_ ) );
    displayer.DisplayItem( tools::translate( "Logistic", "State:" ), tools::ToString( nState_ ) );
    if( pLogProvidingConvoyResourcesEntity_ )
        displayer.DisplayItem( tools::translate( "Logistic", "Transporters provider:" ), extractor.GetDisplayName( *pLogProvidingConvoyResourcesEntity_ ) );
    if( currentStateEndTick_ == std::numeric_limits< unsigned int >::max() )
        displayer.DisplayItem( tools::translate( "Logistic", "Current state end :" ), tools::translate( "Logistic", "Unknown" ) );
    else
    {
        unsigned int endSeconds = simulation_.GetInitialDateTime().toTime_t() + currentStateEndTick_ * simulation_.GetTickDuration();
        QDateTime endDate = QDateTime::fromTime_t( endSeconds );
        QDateTime curDate = simulation_.GetDateTime();

        QString dateDisplay;
        if( endDate.date() != curDate.date() )
            dateDisplay += endDate.date().toString() + " ";
        dateDisplay += endDate.time().toString();

        displayer.DisplayItem( tools::translate( "Logistic", "Current state end :" ), dateDisplay );
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogSupplyConsign::Draw( const Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !pLogHandlingEntity_ || !tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    Point2f from = logistic_helpers::GetLogisticPosition( *pLogHandlingEntity_, true );
    if( from.IsZero() )
        return;
    for( tools::Iterator< const SupplyRecipientResourcesRequest& > it = CreateIterator(); it.HasMoreElements(); )
    {
        const Point2f to = logistic_helpers::GetLogisticPosition( it.NextElement().recipient_, true );
        if( to.IsZero() )
            return;
        if( viewport.IsVisible( Rectangle2f( from, to ) ) )
        {
            glColor4f( COLOR_ORANGE );
            switch( nState_ )
            {
                case eLogSupplyHandlingStatus_ConvoiDeplacementVersPointChargement:
                case eLogSupplyHandlingStatus_ConvoiDeplacementVersPointDechargement:
                    glLineStipple( 1, tools.StipplePattern() );
                    break;
                case eLogSupplyHandlingStatus_ConvoiDeplacementRetour:
                    glLineStipple( 1, tools.StipplePattern(-1) );
                    break;
                default:
                    glLineStipple( 1, tools.StipplePattern(0) );
            }
            tools.DrawCurvedArrow( from, to, 0.6f );
        }
        from = to;
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::FindLogEntity
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogSupplyConsign::FindLogEntity(const sword::ParentEntity& msg)
{
    kernel::Entity_ABC* retval = 0;
    if( msg.has_automat() )
        retval = resolver_.Find( msg.automat().id() );
    else if( msg.has_formation() )
        retval = formationResolver_.Find( msg.formation().id() );
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign::FindLogEntityID
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
unsigned int LogSupplyConsign::FindLogEntityID(const sword::ParentEntity& msg)
{
    unsigned int retval = 0;
    if( msg.has_automat() )
        retval = msg.automat().id() ;
    else if( msg.has_formation() )
        retval = msg.formation().id() ;
    return retval;
}
