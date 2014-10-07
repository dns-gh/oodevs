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
    : Param_ABC ( builder, parameter )
    , converter_( builder.GetStaticModel().coordinateConverter_ )
    , layer_    ( builder.GetParameterLayer() )
    , location_ ()
{
    pPosLabel_ = new QLabel( "---" );
    pPosLabel_->setMinimumWidth( 100 );
    pPosLabel_->setAlignment( Qt::AlignCenter );
    ActivateSwitchEditor( parameter_.GetType() == "point" );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC destructor
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
ParamLocation_ABC< BaseParameter >::~ParamLocation_ABC()
{
    if( location_ )
        location_.reset();
    layer_->Reset();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::BuildInterface
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
QWidget* ParamLocation_ABC< BaseParameter >::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->addWidget( pPosLabel_ );
    if( needSwitchEditor_ )
        layout->addWidget( CreateSwitchEditor() );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Draw
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Draw( const geometry::Point2f& , const ::gui::Viewport_ABC& , ::gui::GlTools_ABC& tools ) const
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
    return location_ && location_->IsValid();
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::NotifyContextMenu
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::NotifyContextMenu( const geometry::Point2f& pos, kernel::ContextMenu& menu )
{
    if( parameter_.GetType() == "point" )
    {
        popupPosition_.reset( new geometry::Point2f( pos ) );
        Param_ABC::CreateMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::NotifyContextMenu
// Created: ABR 2012-02-21
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& menu )
{
    if( parameter_.GetType() != "point" )
    {
        popupPosition_.reset();
        Param_ABC::CreateMenu( menu );
    }
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
        if( group_ && IsOptional() )
            group_->setChecked( true );
        NotifyChange();
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Handle
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::OnMenuClick()
{
    if( parameter_.GetType() == "point" && popupPosition_ )
        layer_->SetPoint( *this, *popupPosition_ );
    else if( parameter_.GetType() == "circle" )
        layer_->StartCircle( *this );
    else if( parameter_.GetType() == "line" )
        layer_->StartLine( *this );
    else if( parameter_.GetType() == "polygon" )
        layer_->StartPolygon( *this );
    else if( parameter_.GetType() == "rectangle" )
        layer_->StartRectangle( *this );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::CommitTo
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && location_ && location_->IsValid() )
        action.AddParameter( *new BaseParameter( parameter_, converter_, *location_ ) );
    else
        action.AddParameter( *new BaseParameter( parameter_, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::InternalVisit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::InternalVisit( const actions::parameters::Location& param )
{
    ActivateOptionalIfNeeded( param );

    if( parameter_.GetType() == "circle" )
        location_.reset( new kernel::Circle() );
    else if( parameter_.GetType() == "line" )
        location_.reset( new kernel::Lines() );
    else if( parameter_.GetType() == "point" )
        location_.reset( new kernel::Point() );
    else if( parameter_.GetType() == "polygon" )
        location_.reset( new kernel::Polygon() );
    else if( parameter_.GetType() == "rectangle" )
        location_.reset( new kernel::Rectangle() );

    pPosLabel_->setText( location_->GetName() );

    const T_PointVector& points = param.GetPoints();
    for( auto it = points.begin(); it != points.end(); ++it )
        location_->AddPoint( *it );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Visit
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Visit( const actions::parameters::Point& param )
{
    InternalVisit( param );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Visit
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Visit( const actions::parameters::Polygon& param )
{
    InternalVisit( param );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Visit
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Visit( const actions::parameters::Location& param )
{
    InternalVisit( param );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation_ABC::Purge
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
template< typename BaseParameter >
void ParamLocation_ABC< BaseParameter >::Purge()
{
    location_.reset();
    pPosLabel_->setText( "---" );
}
