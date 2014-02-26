// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostTemplateElement.h"
#include "GhostCommandPostAttributes.h"
#include "GhostModel.h"

#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement constructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
GhostTemplateElement::GhostTemplateElement( GhostModel& ghosts,
                                            const kernel::Ghost_ABC& ghost )
    : TemplateElement( ghost )
    , ghosts_( ghosts )
    , ghost_( new kernel::GhostPrototype() )
    , isCommandPost_( tools::IsCommandPost( ghost ) )
    , isLogisticBase_( logistic_helpers::IsLogisticBase( ghost ) )
    , fromUnknownType_( false )
{
    ghost_->ghostType_ = ghost.GetGhostType();
    ghost_->name_ = ghost.GetName();
    ghost_->type_ = ghost.GetType();
    ghost_->symbol_ = ghost.GetSymbol();
    ghost_->nature_ = ghost.GetNature();
    ghost_->level_ = ghost.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement constructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
GhostTemplateElement::GhostTemplateElement( GhostModel& ghosts,
                                            xml::xistream& xis )
    : TemplateElement( xis )
    , ghosts_( ghosts )
    , ghost_( new kernel::GhostPrototype() )
    , isCommandPost_( xis.attribute( "commandPost", false ) )
    , isLogisticBase_( xis.attribute( "logisticBase", false ) )
    , fromUnknownType_( false )
{
    std::string subGhostType;
    xis >> xml::attribute( "ghostType", ghost_->type_ )
        >> xml::attribute( "subGhostType", subGhostType )
        >> xml::attribute( "symbol", ghost_->symbol_ )
        >> xml::attribute( "nature", ghost_->nature_ )
        >> xml::attribute( "level", ghost_->level_ );
    ghost_->ghostType_ = ENT_Tr::ConvertToGhostType( subGhostType, ENT_Tr::eToSim );
    ghost_->name_ = name_;
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement constructor
// Created: ABR 2014-02-25
// -----------------------------------------------------------------------------
GhostTemplateElement::GhostTemplateElement( GhostModel& ghosts,
                                            E_GhostType ghostType,
                                            xml::xistream& xis )
    : TemplateElement( xis )
    , ghosts_( ghosts )
    , ghost_( new kernel::GhostPrototype() )
    , isCommandPost_( xis.attribute( "commandPost", false ) )
    , isLogisticBase_( false )
    , fromUnknownType_( true )
{
    ghost_->name_ = name_;
    ghost_->ghostType_ = ghostType;
    ghost_->symbol_ = "symbols/s*gpu";
    ghost_->nature_ = "undefined/undefined/undefined/undefined";
    ghost_->level_ = ghostType == eGhostType_Automat ? "ii" : "ooo";
    if( ghostType == eGhostType_Agent )
        xis >> xml::attribute( "agentType", ghost_->type_ );
    else if( ghostType == eGhostType_Automat )
    {
        xis >> xml::attribute( "automatType", ghost_->type_ )
            >> xml::optional >> xml::attribute( "symbol", ghost_->symbol_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement destructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
GhostTemplateElement::~GhostTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement::Instanciate
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
kernel::Entity_ABC* GhostTemplateElement::Instanciate( kernel::Entity_ABC& superior,
                                                       const geometry::Point2f& center,
                                                       ColorController& colorController )
{
    if( fromUnknownType_ )
    {
        tools::SetKarma( superior, ghost_->symbol_ );
        tools::SetLevel( superior, ghost_->level_ );
    }
    kernel::Ghost_ABC* ghost = ghosts_.Create( superior, *ghost_, center );
    if( ghost )
    {
        SetColor( *ghost, colorController );
        if( isCommandPost_ && ghost_->ghostType_ == eGhostType_Agent )
        {
            auto& commandPostAttribute = static_cast< GhostCommandPostAttributes& >( ghost->Get< kernel::CommandPostAttributes_ABC >() );
            commandPostAttribute.Update( true );
        }
        else if( isLogisticBase_ && ghost_->ghostType_ == eGhostType_Automat )
            ghost->Get< gui::LogisticBase >().SetBase( true );
    }
    return ghost;
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement::Serialize
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
void GhostTemplateElement::Serialize( xml::xostream& xos ) const
{
    TemplateElement::Serialize( xos );
    xos << xml::attribute( "type", "ghost" )
        << xml::attribute( "ghostType", ghost_->type_ )
        << xml::attribute( "subGhostType", ENT_Tr::ConvertFromGhostType( ghost_->ghostType_, ENT_Tr::eToSim ) )
        << xml::attribute( "symbol", ghost_->symbol_ )
        << xml::attribute( "nature", ghost_->nature_ )
        << xml::attribute( "level", ghost_->level_ );
    if( ghost_->ghostType_ == eGhostType_Agent )
        xos << xml::attribute( "commandPost", isCommandPost_ );
    else if( ghost_->ghostType_ == eGhostType_Automat )
        xos << xml::attribute( "logisticBase", isLogisticBase_ );
}

// -----------------------------------------------------------------------------
// Name: GhostTemplateElement::IsCompatible
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
bool GhostTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return ghost_->ghostType_ == eGhostType_Agent &&
           superior.GetTypeName() == kernel::Automat_ABC::typeName_ ||
           ghost_->ghostType_ == eGhostType_Automat &&
           superior.GetTypeName() == kernel::Formation_ABC::typeName_;
}
