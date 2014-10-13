// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Action_ABC.h"
#include "Parameter_ABC.h"
#include "ActionTasker.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/ActionController.h"
#include "protocol/Protocol.h"

using namespace actions;

namespace
{
    unsigned long ids = 0;
    const std::string typeName = "action";
}

// -----------------------------------------------------------------------------
// Name: Action_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::Action_ABC( kernel::Controller& controller, const kernel::OrderType* type )
    : controller_( controller )
    , type_( type )
    , id_( ++ids )
    , name_( type ? type->GetName().c_str() : "" )
    , valid_( type != nullptr )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::~Action_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetTypeName
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
const std::string& Action_ABC::GetTypeName() const
{
    return typeName;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetId
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
unsigned long Action_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
QString Action_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Rename
// Created: SBO 2008-05-15
// -----------------------------------------------------------------------------
void Action_ABC::Rename( const QString& name )
{
    name_ = name;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetType
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const kernel::OrderType* Action_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::AddParameter
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void Action_ABC::AddParameter( Parameter_ABC& parameter )
{
    ParameterContainer_ABC::Register( Count(), parameter );
    controller_.Update( *this );
}

namespace
{
    struct BarycenterComputer
    {
        BarycenterComputer() : count_( 0 ) {}
        void AddPoint( const geometry::Point2f& point )
        {
            if( !point.IsZero() )
            {
                result_ += geometry::Vector2f( point.X(), point.Y() );
                ++count_;
            }
        }
        geometry::Point2f Result() const
        {
            if( count_ )
                return geometry::Point2f( result_.X() / count_, result_.Y() / count_ );
            return geometry::Point2f();
        }
        geometry::Point2f result_;
        unsigned int count_;
    };
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Draw
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void Action_ABC::Draw( const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    BarycenterComputer computer;
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        computer.AddPoint( it->second->GetPosition() );
    auto where = computer.Result();
    if( where.IsZero() )
        if( auto tasker = Retrieve< ActionTasker >() )
            if( auto entity = tasker->GetTasker() )
                if( auto positions = entity->Retrieve< kernel::Positions >() )
                    where = positions->GetPosition();
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void Action_ABC::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( where, viewport, tools );

    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Display
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void Action_ABC::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Action", "Parameter" ) ).Display( GetName() )
             .Item( tools::translate( "Action", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Action_ABC::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_.toStdString() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "parameter" );
        it->second->Serialize( xos );
        xos << xml::end;
    }
    GetInterfaces().Apply( &kernel::Serializable_ABC::SerializeAttributes, xos );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool Action_ABC::CheckKnowledgeValidity() const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( ! ( it->second->CheckKnowledgeValidity() ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Action_ABC::CommitTo( sword::MissionParameters& message ) const
{
    // $$$$ FHD 2009-10-28: potential bug, parameters serialized in "map" order
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( *message.add_elem() );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Action_ABC::Clean( sword::MissionParameters& message ) const
{
    message.Clear();
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Select
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Action_ABC::MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    controller.MultipleSelect( elements );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::ContextMenu
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::ContextMenu( kernel::ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, where );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Activate
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::Activate( kernel::ActionController& controller ) const
{
    controller.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Invalidate
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
void Action_ABC::Invalidate()
{
    valid_ = false;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::IsValid
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
bool Action_ABC::IsValid() const
{
    return valid_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::CheckValidity
// Created: ABR 2014-01-22
// -----------------------------------------------------------------------------
bool Action_ABC::CheckValidity() const
{
    const ActionTasker* tasker = Retrieve< ActionTasker >();
    bool taskerValidity = tasker ? tasker->GetTasker() != 0 : true;
    bool paramValidity = true;
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        paramValidity = paramValidity && ( it->second->IsOptional() || it->second->IsSet() );
    return valid_ && taskerValidity && paramValidity && CheckKnowledgeValidity();
}
