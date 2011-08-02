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
#include "moc_LimitParameter.cpp"
#include "actions/Limit.h"
#include "actions/Action_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_gui/RichLabel.h"
#include "ENT/Ent_Tr.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: LimitParameter constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
LimitParameter::LimitParameter( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller )
    : QObject    ( parent )
    , Param_ABC  ( ENT_Tr::ConvertFromActionParameter( ENT_Tr::ConvertToActionParameter( parameter.GetName().c_str() ), ENT_Tr_ABC::eToTr ).c_str() )
    , controller_( controller )
    , parameter_ ( parameter )
    , converter_ ( converter )
    , pLabel_    ( 0 )
    , potential_ ( 0 )
    , selected_  ( 0 )
{
    controller_.Register( *this );
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
    Q3HBox* box = new Q3HBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), false, box );
    entityLabel_ = new QLabel( "---", box );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    box->setStretchFactor( entityLabel_, 1 );
    return box;
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::CheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool LimitParameter::CheckValidity()
{
    if( ! parameter_.IsOptional() && !selected_ )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
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
        int id = menu.InsertItem( "Parameter", tools::translate( "LimitParameter", "Set %1" ).arg( GetName() ), this, SLOT( MenuItemValidated() ) );
        menu.SetChecked( id, selected_ != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::MenuItemValidated
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void LimitParameter::MenuItemValidated()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" ); // $$$$ AGE 2006-03-14: use a displayer
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
    kernel::Lines lines;
    if( selected_ )
    {
        GeometrySerializer serializer( lines, converter_ );
        selected_->Get< kernel::Positions >().Accept( serializer );
    }
    std::auto_ptr< actions::parameters::Limit > param( new actions::parameters::Limit( parameter_, converter_, lines ) );
    parameter.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: LimitParameter::IsOptional
// Created: SBO 2008-03-06
// -----------------------------------------------------------------------------
bool LimitParameter::IsOptional() const
{
    return parameter_.IsOptional();
}
