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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/AgentTypes.h"
#include "FormationTemplateElement.h"
#include "AutomatTemplateElement.h"
#include "AgentTemplateElement.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
HierarchyTemplate::HierarchyTemplate( AgentsModel& agents, FormationModel& formations, const kernel::Entity_ABC& base, bool root, ColorController& colorController )
    : name_           ( "" )
    , colorController_( colorController )
{
    element_.reset( CreateElement( agents, formations, base ) );
    if( element_.get() )
        name_ = element_->GetName();
    if( const TacticalHierarchies* hierarchies = base.Retrieve< TacticalHierarchies >() )
    {
        tools::Iterator< const Entity_ABC& > children = hierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            subTemplates_.push_back( new HierarchyTemplate( agents, formations, children.NextElement(), false, colorController_ ) );
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
HierarchyTemplate::HierarchyTemplate( AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, xml::xistream& input, ColorController& colorController )
    : name_           ( "" )
    , colorController_( colorController )
{
    element_.reset( CreateElement( agents, formations, types, input ) );
    input >> xml::list( "template", *this, &HierarchyTemplate::ReadSubTemplate, agents, formations, types );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::ReadSubTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void HierarchyTemplate::ReadSubTemplate( xml::xistream& input, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types )
{
    subTemplates_.push_back( new HierarchyTemplate( agents, formations, types, input, colorController_ ) );
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
void HierarchyTemplate::Serialize( xml::xostream& output ) const
{
    output << xml::start( "template" );
    if( element_.get() )
    {
        output << xml::start( "element" )
               << xml::attribute( "x", referencePosition_.X() )
               << xml::attribute( "y", referencePosition_.Y() )
               << xml::attribute( "name", name_.toStdString() );
        element_->Serialize( output );
        output << xml::end;
    }
    for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
        (*it)->Serialize( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void HierarchyTemplate::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center ) const
{
    if( element_.get() )
    {
        geometry::Point2f position( referencePosition_.X() + center.X(),
                                    referencePosition_.Y() + center.Y() );
        kernel::Entity_ABC* entity = element_->Instanciate( superior, position, colorController_ );
        if( entity )
            for( auto it = subTemplates_.begin(); it != subTemplates_.end(); ++it )
                (*it)->Instanciate( *entity, center );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::CreateElement
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
TemplateElement_ABC* HierarchyTemplate::CreateElement( AgentsModel& agents, FormationModel& formations, const kernel::Entity_ABC& entity )
{
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
    {
        referencePosition_ = agent->Get< Positions >().GetPosition();
        return new AgentTemplateElement( agents, *agent );
    }
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        return new AutomatTemplateElement( agents, *automat );
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        return new FormationTemplateElement( formations, *formation );
    return 0;
}

namespace
{
    template< typename T >
    const T& ReadType( const tools::Resolver_ABC< T, std::string >& types, xml::xistream& input, const std::string& xmlTag )
    {
        return types.Get( input.attribute< std::string >( xmlTag ) );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::CreateElement
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
TemplateElement_ABC* HierarchyTemplate::CreateElement( AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, xml::xistream& input )
{
    std::string type, name;
    float x = 0, y = 0;
    input >> xml::optional
          >> xml::start( "element" )
             >> xml::attribute( "type", type )
             >> xml::attribute( "x", x )
             >> xml::attribute( "y", y )
             >> xml::attribute( "name", name );
    name_ = name.c_str();

    referencePosition_.Set( x, y );
    TemplateElement_ABC* result = 0;
    if( type == "formation" )
        result = new FormationTemplateElement( formations, input );
    else if( type == "automat" )
        result = new AutomatTemplateElement( agents, ReadType< AutomatType >( types, input, "automatType" ), input );
    else if( type == "agent" )
        result = new AgentTemplateElement( agents, ReadType< AgentType >( types, input, "agentType" ), input );
    input >> xml::end;
    return result;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool HierarchyTemplate::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return element_.get() && element_->IsCompatible( superior );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::Rename
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void HierarchyTemplate::Rename( const QString& name )
{
    name_ = name;
    if( element_.get() )
        element_->Rename( name );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTemplate::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString HierarchyTemplate::GetName() const
{
    return element_.get() ? element_->GetName() : QString();
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
