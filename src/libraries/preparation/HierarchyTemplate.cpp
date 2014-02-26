// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "HierarchyTemplate.h"
#include "AgentTemplateElement.h"
#include "AutomatTemplateElement.h"
#include "FormationTemplateElement.h"
#include "GhostTemplateElement.h"

#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

namespace
{
    template< typename T >
    const T* ReadType( const tools::Resolver_ABC< T, std::string >& types,
                       xml::xistream& xis,
                       const std::string& xmlTag )
    {
        return types.Find( xis.attribute< std::string >( xmlTag ) );
    }

    TemplateElement* CreateElement( AgentsModel& agents,
                                        FormationModel& formations,
                                        GhostModel& ghosts,
                                        const Entity_ABC& entity,
                                        geometry::Point2f& position )
    {
        if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &entity ) )
        {
            position = agent->Get< Positions >().GetPosition();
            return new AgentTemplateElement( agents, *agent );
        }
        if( const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &entity ) )
            return new AutomatTemplateElement( agents, *automat );
        if( const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &entity ) )
            return new FormationTemplateElement( formations, *formation );
        if( const Ghost_ABC* ghost = dynamic_cast< const Ghost_ABC* >( &entity ) )
        {
            position = ghost->Get< Positions >().GetPosition();
            return new GhostTemplateElement( ghosts, *ghost );
        }
        return 0;
    }

    TemplateElement* CreateElement( AgentsModel& agents,
                                        FormationModel& formations,
                                        GhostModel& ghosts,
                                        const AgentTypes& types,
                                        xml::xistream& xis,
                                        geometry::Point2f& position )
    {
        std::string type, name;
        float x = 0, y = 0;
        xis >> xml::optional
            >> xml::start( "element" )
               >> xml::attribute( "type", type )
               >> xml::attribute( "x", x )
               >> xml::attribute( "y", y );
        position.Set( x, y );
        TemplateElement* result = 0;
        if( type == "formation" )
            result = new FormationTemplateElement( formations, xis );
        else if( type == "automat" )
        {
            if( auto automatType = ReadType< AutomatType >( types, xis, "automatType" ) )
                result = new AutomatTemplateElement( agents, *automatType, xis );
            else
                result = new GhostTemplateElement( ghosts, eGhostType_Automat, xis );
        }
        else if( type == "agent" )
        {
            if( auto agentType = ReadType< AgentType >( types, xis, "agentType" ) )
                result = new AgentTemplateElement( agents, *agentType, xis );
            else
                result = new GhostTemplateElement( ghosts, eGhostType_Agent, xis );
        }
        else if( type == "ghost" )
            result = new GhostTemplateElement( ghosts, xis );
        xis >> xml::end;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
HierarchyTemplate::HierarchyTemplate( AgentsModel& agents,
                                      FormationModel& formations,
                                      GhostModel& ghosts,
                                      const Entity_ABC& base,
                                      bool root,
                                      ColorController& colorController )
    : colorController_( colorController )
{
    element_.reset( CreateElement( agents,
                                   formations,
                                   ghosts,
                                   base,
                                   referencePosition_ ) );
    if( const TacticalHierarchies* hierarchies = base.Retrieve< TacticalHierarchies >() )
    {
        tools::Iterator< const Entity_ABC& > children = hierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            subTemplates_.push_back( new HierarchyTemplate( agents, formations, ghosts, children.NextElement(), false, colorController_ ) );
            referencePosition_.Set( referencePosition_.X() + subTemplates_.back()->referencePosition_.X(),
                                    referencePosition_.Y() + subTemplates_.back()->referencePosition_.Y() );
        }
        if( ! subTemplates_.empty() )
            referencePosition_.Set( referencePosition_.X() / subTemplates_.size(),
                                    referencePosition_.Y() / subTemplates_.size() );
    }
    if( root )
        SetBasePosition( referencePosition_ );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
HierarchyTemplate::HierarchyTemplate( AgentsModel& agents,
                                      FormationModel& formations,
                                      GhostModel& ghosts,
                                      const AgentTypes& types,
                                      xml::xistream& xis,
                                      ColorController& colorController )
    : colorController_( colorController )
{
    element_.reset( CreateElement( agents,
                                   formations,
                                   ghosts,
                                   types,
                                   xis,
                                   referencePosition_ ) );
    bool isGhost = element_ && dynamic_cast< GhostTemplateElement* >( element_.get() ) != 0;
    if( !isGhost )
        xis >> xml::list( "template", [&] ( xml::xistream& subXis )
            {
                subTemplates_.push_back( new HierarchyTemplate( agents,
                                                                formations,
                                                                ghosts,
                                                                types,
                                                                subXis,
                                                                colorController_ ) );
            } );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
HierarchyTemplate::~HierarchyTemplate()
{
    for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void HierarchyTemplate::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "template" );
    if( element_ )
    {
        xos << xml::start( "element" )
                << xml::attribute( "x", referencePosition_.X() )
                << xml::attribute( "y", referencePosition_.Y() )
                << xml::attribute( "name", element_->GetName() );
        element_->Serialize( xos );
        xos << xml::end;
    }
    for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
        (*it)->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void HierarchyTemplate::Instanciate( Entity_ABC& superior,
                                     const geometry::Point2f& center ) const
{
    if( element_ )
    {
        geometry::Point2f position( referencePosition_.X() + center.X(),
                                    referencePosition_.Y() + center.Y() );
        Entity_ABC* entity = element_->Instanciate( superior, position, colorController_ );
        if( entity )
            for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
                (*it)->Instanciate( *entity, center );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool HierarchyTemplate::IsCompatible( const Entity_ABC& superior ) const
{
    return element_ && element_->IsCompatible( superior );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::Rename
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void HierarchyTemplate::Rename( const QString& name )
{
    if( element_ )
        element_->Rename( name );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString HierarchyTemplate::GetName() const
{
    return element_ ? element_->GetName() : QString();
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::SetBasePosition
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void HierarchyTemplate::SetBasePosition( geometry::Point2f center )
{
    for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
        (*it)->SetBasePosition( center );
    referencePosition_.Set( referencePosition_.X() - center.X(),
                            referencePosition_.Y() - center.Y() );
}
