// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC constructor
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
ParamLocation_ABC< BaseParameter >::ParamLocation_ABC( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC ( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , converter_( builder.GetStaticModel().coordinateConverter_ )
    , layer_    ( builder.GetParameterLayer() )
    , location_ ()
{
    pPosLabel_ = new QLabel( "---" );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC destructor
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
ParamLocation_ABC< BaseParameter >::~ParamLocation_ABC()
{
    if( location_.get() )
        location_.reset();
    layer_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::BuildInterface
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
QWidget* ParamLocation_ABC< BaseParameter >::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    layout->addWidget( pPosLabel_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Draw
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& , const kernel::GlTools_ABC& tools ) const
{
    if( InternalCheckValidity() )
        ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::InternalCheckValidity
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
bool ParamLocation_ABC< BaseParameter >::InternalCheckValidity() const
{
    return location_.get() && location_->IsValid();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::InternalOnMenuClick
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::InternalOnMenuClick()
{
    if( group_ && IsOptional() )
        group_->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::NotifyContextMenu
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& menu )
{
    Param_ABC::CreateMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Handle
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
    {
        location_.reset( &location );
        pPosLabel_->setText( location.GetName() );
        NotifyChange();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Handle
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::OnMenuClick()
{
    if( parameter_.GetType() == "circle" )
        layer_.StartCircle( *this );
    else if( parameter_.GetType() == "line" )
        layer_.StartLine( *this );
    else if( parameter_.GetType() == "point" )
        layer_.StartPoint( *this );
    else if( parameter_.GetType() == "polygon" )
        layer_.StartPolygon( *this );
    else if( parameter_.GetType() == "rectangle" )
        layer_.StartRectangle( *this );
    InternalOnMenuClick();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::CommitTo
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && location_.get() && location_->IsValid() )
        action.AddParameter( *new BaseParameter( parameter_, converter_, *location_ ) );
    else
        action.AddParameter( *new BaseParameter( parameter_, converter_ ) );
}
