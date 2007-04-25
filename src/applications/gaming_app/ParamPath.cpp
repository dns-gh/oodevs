// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/RichLabel.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLocation.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( &parameter )
    , converter_( converter )
    , layer_( layer )
    , positions_( agent.Get< Positions >() )
    , pLabel_( 0 )
    , location_( 0 )
    , optional_( parameter.IsOptional() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
ParamPath::ParamPath( QObject* parent, const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent, bool optional )
    : QObject( parent )
    , Param_ABC( name )
    , parameter_( 0 )
    , converter_( converter )
    , layer_( layer )
    , positions_( agent.Get< Positions >() )
    , pLabel_( 0 )
    , location_( 0 )
    , optional_( optional )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new RichLabel( GetName(), false, box );
    pPosLabel_ = new QLabel( "---", box );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    pPosLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const GlTools_ABC& tools ) const
{
    if( location_ )
        location_->Draw( tools );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CheckValidity
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
bool ParamPath::CheckValidity()
{
    if( ! optional_ && ( ! location_ || !location_->IsValid() ) )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( ! pLabel_ )
        InterfaceNotInitialized();
    asn.value.t = T_MissionParameter_value_itineraire;
    asn.value.u.itineraire = new ASN1T_Itineraire();
    CommitTo( *asn.value.u.itineraire );
    asn.null_value = asn.value.u.itineraire->vecteur_point.n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPath::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.itineraire )
        delete[] asn.value.u.itineraire->vecteur_point.elem;
    delete asn.value.u.itineraire;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( ASN1T_Itineraire& destination ) const
{
    if( location_ )
    {
        LocationSerializer serializer( converter_ );
        serializer.Serialize( *location_, destination );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamPath::CommitTo( Action_ABC& action ) const
{
    if( !parameter_ )
        throw std::runtime_error( "OrderParameter not defined" );
    std::auto_ptr< ActionParameterLocation > param( new ActionParameterLocation( *parameter_, converter_, *location_ ) );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::NotifyContextMenu( const kernel::Nothing&, ContextMenu& menu )
{   
    menu.InsertItem( "Parameter", GetName(), this, SLOT( StartPath() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    layer_.StartPath( *this, positions_ );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Handle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParamPath::Handle( Location_ABC& location )
{
    if( location.IsValid() )
    {
        delete location_;
        location_ = &location;
        pPosLabel_->setText( location.GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::Show()
{
    pLabel_->parentWidget()->show();
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPath::Hide()
{
    pLabel_->parentWidget()->hide();
}
