// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAtlasNature.h"
#include "moc_ParamAtlasNature.cpp"
#include "actions/Action_ABC.h"
#include "actions/AtlasNature.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"
#include <Qt3Support/q3buttongroup.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::ParamAtlasNature( QObject* parent, const kernel::OrderParameter& parameter, const kernel::AtlasNatures& natures )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , natures_  ( natures )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::~ParamAtlasNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamAtlasNature::BuildInterface( QWidget* parent )
{
    Q3ButtonGroup* group = new Q3ButtonGroup( 2, Qt::Horizontal, GetName(), parent );
    tools::Iterator< const kernel::AtlasNature& > it( natures_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::AtlasNature& nature = it.NextElement();
        new QCheckBox( nature.GetName(), group );
        fields_.push_back( &nature );
    }
    connect( group, SIGNAL( clicked( int ) ), SLOT( OnClicked( int ) ) );
    return group;
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::CommitTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ParamAtlasNature::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter( *new actions::parameters::AtlasNature( parameter_, nature_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::OnClicked
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamAtlasNature::OnClicked( int id )
{
    nature_.Toggle( *fields_[id] );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamAtlasNature::IsOptional() const
{
    return parameter_.IsOptional();
}
