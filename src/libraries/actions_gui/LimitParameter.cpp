// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "LimitParameter.h"
#include "InterfaceBuilder_ABC.h"
#include "actions/Limit.h"
#include "actions/Action_ABC.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "ENT/Ent_Tr.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: LimitParameter constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::LimitParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC  ( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , controller_( builder.GetControllers().controller_ )
    , converter_ ( builder.GetStaticModel().coordinateConverter_ )
    , potential_ ( 0 )
    , selected_  ( 0 )
{
    controller_.Register( *this );
    name_ = ENT_Tr::ConvertFromActionParameter( ENT_Tr::ConvertToActionParameter( parameter.GetName().c_str() ), ENT_Tr_ABC::eToTr ).c_str();
    if( name_.isEmpty() )
        name_ = parameter_.GetName().c_str();
}

// -----------------------------------------------------------------------------
// Name: LimitParameter destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::~LimitParameter()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* LimitParameter::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    entityLabel_ = new QLabel( "---", parent );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    layout->addWidget( entityLabel_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::InternalCheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool LimitParameter::InternalCheckValidity() const
{
    return selected_ != 0;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::NotifyContextMenu
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() )
    {
        potential_ = &entity;
        kernel::ContextMenu::T_MenuVariant variant = Param_ABC::CreateMenu( menu );
        if( QAction* const* vAction = boost::get< QAction* >( &variant ) )
        {
            if( *vAction )
            {
                ( *vAction )->setCheckable( true );
                ( *vAction )->setChecked( selected_ != 0 );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::MenuItemValidated
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::OnMenuClick()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" ); // $$$$ AGE 2006-03-14: use a displayer
    if( group_ && IsOptional() )
        group_->setChecked( selected_ != 0 );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::NotifyDeleted
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void LimitParameter::NotifyDeleted( const kernel::TacticalLine_ABC& entity )
{
    if( &entity == potential_ )
        potential_ = 0;
    if( &entity == selected_ )
    {
        selected_ = 0;
        Display( "---" );
    }
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::Display
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::Display( const QString& what )
{
    entityLabel_->setText( what );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::Draw
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void LimitParameter::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !selected_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4f( 1.f, 1.f, 1.f, 1.f );
        selected_->Interface().Apply( &Drawable_ABC::Draw, point, viewport, tools );
    glPopAttrib();
}

namespace
{
    struct GeometrySerializer : public kernel::LocationVisitor_ABC
    {
        GeometrySerializer( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter )
            : converter_( converter )
            , location_( location )
        {}

        virtual void VisitLines( const T_PointVector& points )
        {
            for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                location_.AddPoint( *it );
        }

        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitPolygon( const T_PointVector& ) {}
        virtual void VisitCircle( const geometry::Point2f&, float ) {}
        virtual void VisitPoint( const geometry::Point2f& ) {}
        virtual void VisitPath( const geometry::Point2f&, const T_PointVector& ) {}

    private:
        GeometrySerializer& operator=( const GeometrySerializer& );

        const kernel::CoordinateConverter_ABC& converter_;
        kernel::Location_ABC& location_;
    };
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LimitParameter::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() && selected_ )
    {
        kernel::Lines lines;
        if( selected_ )
        {
            GeometrySerializer serializer( lines, converter_ );
            selected_->Get< kernel::Positions >().Accept( serializer );
        }
        std::auto_ptr< actions::parameters::Limit > param( new actions::parameters::Limit( parameter_, converter_, lines ) );
        parameter.AddParameter( *param.release() );
    }
    else
        parameter.AddParameter( *new actions::parameters::Limit( parameter_, converter_ ) );
}
