// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, Entity_ABC& entity, ActionController& controller )
    : QVBox      ( parent )
    , controller_( controller )
    , entity_    ( entity )
{
    setMargin( 5 );
    setSpacing( 4 );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->RemoveFromController();
    for( CIT_Parameters it = orderContext_.begin(); it != orderContext_.end(); ++it )
        (*it)->RemoveFromController();
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
    for( CIT_Parameters it = orderContext_.begin(); it != orderContext_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    for( CIT_Parameters it = orderContext_.begin(); it != orderContext_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    return b;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::IsEmpty() const
{
    return parameters_.empty() && orderContext_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::GetEntity
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& MissionInterface_ABC::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateTitle
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateTitle( const QString& title )
{
    QLabel* label = new QLabel( title, this );
    label->setFrameStyle( QFrame::Box | QFrame::Sunken );
    label->setAlignment( Qt::AlignCenter );
    QFont font = label->font();
    font.setBold( true );
    label->setFont( font );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateOkCancelButtons
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateOkCancelButtons()
{
    QHBox* pBox = new QHBox( this );
    QPushButton* pOk = new QPushButton( tr( "Ok" ), pBox );
    QPushButton* pCancel = new QPushButton( tr( "Cancel" ), pBox );
    pOk->setDefault( true );

    connect( pOk, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancel, SIGNAL( clicked() ), parent(), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( Param_ABC& parameter, bool optional )
{
    parameters_.push_back( &parameter );
    parameter.SetOptional( optional );
    parameter.BuildInterface( this );
    parameter.RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddOrderContext
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddOrderContext( Param_ABC& parameter, bool optional )
{
    orderContext_.push_back( &parameter );
    parameter.SetOptional( optional );
    parameter.BuildInterface( this );
    parameter.RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( ASN1T_MissionParameters& asn ) const
{
    asn.n = parameters_.size();
    asn.elem = new ASN1T_MissionParameter[asn.n];
    unsigned int i = 0;
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end(); ++it, ++i )
        (*it)->CommitTo( asn.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( ASN1T_OrderContext& asn ) const
{
    for( CIT_Parameters it = orderContext_.begin() ; it != orderContext_.end(); ++it )
        (*it)->CommitTo( asn );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Clean( ASN1T_MissionParameters& asn ) const
{
    unsigned int i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it, ++i )
        (*it)->Clean( asn.elem[i] );
    delete[] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Clean( ASN1T_OrderContext& asn ) const
{
    for( CIT_Parameters it = orderContext_.begin(); it != orderContext_.end(); ++it )
        (*it)->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( Action_ABC& action ) const
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CommitTo( action, false );
    for( CIT_Parameters it = orderContext_.begin(); it != orderContext_.end(); ++it )
        (*it)->CommitTo( action, true );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( const GlTools_ABC& tools, Viewport_ABC& extent ) const
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
    {
        const geometry::Point2f p = entity_.Get< Positions >().GetPosition();
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
    for( CIT_Parameters it = orderContext_.begin() ; it != orderContext_.end() ; ++it )
    {
        const geometry::Point2f p = entity_.Get< Positions >().GetPosition();
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::OnOk
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void MissionInterface_ABC::OnOk()
{
    if( !CheckValidity() )
        return;
    Publish();
    parentWidget()->hide();
}
