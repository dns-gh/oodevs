// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAtlasNature.h"
#include "moc_ParamAtlasNature.cpp"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterAtlasNature.h"
#include "gaming/AtlasNatures.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAtlasNature::ParamAtlasNature( QObject* parent, const OrderParameter& parameter, const AtlasNatures& natures )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , natures_( natures )
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
void ParamAtlasNature::BuildInterface( QWidget* parent )
{
    QButtonGroup* group = new QButtonGroup( 2, Qt::Horizontal, GetName(), parent );
    Iterator< const AtlasNature& > it( natures_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const AtlasNature& nature = it.NextElement();
        new QCheckBox( nature.GetName(), group );
        fields_.push_back( &nature );
    }
    connect( group, SIGNAL( clicked( int ) ), SLOT( OnClicked( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamAtlasNature::CommitTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ParamAtlasNature::CommitTo( ActionParameterContainer_ABC& action ) const
{
    action.AddParameter( *new ActionParameterAtlasNature( parameter_, nature_ ) );
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
