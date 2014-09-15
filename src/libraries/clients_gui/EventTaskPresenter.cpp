// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventTaskPresenter.h"
#include "moc_EventTaskPresenter.cpp"
#include "Event.h"
#include "EventTaskViewState.h"
#include "EventView_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include <boost/property_tree/ptree.hpp>
#include <timeline/api.h>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace gui;
namespace bpt = boost::property_tree;

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter constructor
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
EventTaskPresenter::EventTaskPresenter( EventView_ABC< EventTaskViewState >& view,
                                        kernel::Controllers& controllers,
                                        const kernel::EntityResolver_ABC& model )
    : EventSubPresenter_ABC< EventTaskViewState >( eEventTypes_Task, view )
    , entity_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter destructor
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
EventTaskPresenter::~EventTaskPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnLabelChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnLabelChanged( const QString& label )
{
    state_->label_ = label.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnDescriptionChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnDescriptionChanged( const QString& description )
{
    state_->description_ = description.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnUrlChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnUrlChanged( const QString& url )
{
    state_->url_ = url.toStdString();
    state_->isUrlValid_ = !state_->url_.empty();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnPayloadChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnPayloadChanged( const QString& payload )
{
    state_->payload_ = payload.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnShowClicked
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnShowClicked()
{
    state_->isPayloadVisible_ = !state_->isPayloadVisible_;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::Trigger
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::Trigger( const gui::Event& /* event */ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::Clear
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::Purge
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::Purge()
{
    EventSubPresenter_ABC< EventTaskViewState >::Purge();
    entity_ = 0;
}

namespace
{
    std::string EntityToMetadata( const kernel::Entity_ABC* entity )
    {
        return QString( "{ \"sword_entity\": %1 }" ).arg( entity ? entity->GetId() : 0 ).toStdString();
    }
    const kernel::Entity_ABC* MetadataToEntity( const std::string& metadata,
                                                const kernel::EntityResolver_ABC& model )
    {
        if( metadata.empty() )
            return nullptr;
        bpt::ptree rpy;
        try
        {
            std::istringstream input( metadata );
            bpt::read_json( input, rpy );
            if( auto tasker = rpy.get_optional< uint32_t >( "sword_entity" ) )
                return model.FindEntity( *tasker );
        }
        catch( ... )
        {
        }
        return nullptr;
    }
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::FillFrom
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventTaskPresenter::FillFrom( const gui::Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    state_->Purge();
    state_->label_ = timelineEvent.name;
    state_->description_ = timelineEvent.info;
    state_->url_ = timelineEvent.action.target;
    const QString data = QByteArray::fromBase64( timelineEvent.action.payload.c_str() );
    state_->bytes_ = data.size();
    state_->payload_ = data.toStdString();
    state_->isUrlValid_ = !state_->url_.empty();
    state_->isPayloadVisible_ = false;
    SetTasker( MetadataToEntity( timelineEvent.metadata, model_ ) );
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::CommitTo
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::CommitTo( timeline::Event& event ) const
{
    event.name = state_->label_;
    event.info = state_->description_;
    event.action.target = state_->url_;
    event.action.apply = false;
    if( !state_->url_.empty() )
    {
        QByteArray data( state_->payload_.c_str() );
        event.action.payload = data.toBase64().data();
    }
    event.metadata = EntityToMetadata( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnTargetChanged
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    SetTasker( entity );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::SetTasker
// Created: ABR 2014-09-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::SetTasker( const kernel::Entity_ABC* entity )
{
    entity_ = entity;
    state_->target_ = entity_ ? entity_->GetId() : 0;
}
