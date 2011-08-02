// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "LimaParameter.h"
#include "moc_LimaParameter.cpp"
#include "ParamDateTime.h"
#include "actions/Lima.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: LimaParameter constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
LimaParameter::LimaParameter( QObject* parent, const QString& name, const kernel::CoordinateConverter_ABC& converter, const QDateTime& currentDate, const kernel::TacticalLine_ABC& line )
    : QObject( parent )
    , Param_ABC( name )
    , converter_  ( converter )
    , currentDate_( currentDate )
    , line_       ( &line )
    , schedule_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaParameter constructor
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
LimaParameter::LimaParameter( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const QDateTime& currentDate )
: QObject( parent )
, Param_ABC( parameter.GetName().c_str() )
, converter_  ( converter )
, currentDate_( currentDate )
, line_       ( 0 )
, schedule_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaParameter destructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
LimaParameter::~LimaParameter()
{
    delete schedule_;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::CheckValidity
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
bool LimaParameter::CheckValidity()
{
    return line_ && ( schedule_ && schedule_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::BuildInterface
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
QWidget* LimaParameter::BuildInterface( QWidget* parent )
{
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, GetName(), parent );
    new QLabel( tools::translate( "LimaParameter", "Line" ), group );
    entityLabel_ = new QLabel( "---", group );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    new QLabel( tools::translate( "LimaParameter", "Functions" ), group );
    functions_ = new Q3ListBox( group );
    functions_->setSelectionMode( Q3ListBox::Multi );
    for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
        functions_->insertItem( tools::ToShortString( (kernel::E_FuncLimaType)i ), i );
    functions_->setRowMode( Q3ListBox::FitToHeight );
    functions_->setFixedSize( 150, functions_->itemHeight( 0 ) * 4 );
    schedule_ = new ParamDateTime( this, tools::translate( "LimaParameter", "Schedule" ), currentDate_, true ); // $$$$ SBO 2007-05-14: optional
    schedule_->BuildInterface( group );
    return group;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::Draw
// Created: SBO 2007-05-02
// Modified: MGD 2010-10-27
// -----------------------------------------------------------------------------
void LimaParameter::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( line_ )
    {
        glPushAttrib( GL_CURRENT_BIT );
            glColor4f( 1, 0, 0, 0.5f );
            line_->Interface().Apply( &kernel::Drawable_ABC::Draw, point, viewport, tools ); // $$$$ SBO 2007-05-02:
            // $$$$ AGE 2007-05-09: pourquoi pas juste Draw ??
            glColor3f( 0.7f, 0, 0 );
            QStringList functions;
            for( unsigned int i = 0; i < functions_->count(); ++i )
                if( functions_->isSelected( i ) )
                    functions.append( tools::ToShortString( (kernel::E_FuncLimaType)i ) );
            const geometry::Point2f position = line_->Get< kernel::Positions >().GetPosition();
            const geometry::Vector2f lineFeed = geometry::Vector2f( 0, -18.f * tools.Pixels() ); // $$$$ SBO 2007-05-15: hard coded \n
            if( ! functions.isEmpty() )
                tools.Print( functions.join( ", " ).ascii(), position + lineFeed, QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
            schedule_->Draw( position + lineFeed * 2.f, viewport, tools );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::MenuItemValidated
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::MenuItemValidated( int index )
{
    entityLabel_->setText( line_->GetName() );
    functions_->setSelected( index, !functions_->isSelected( index ) );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::NotifyContextMenu
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() )
        return;
    if( !line_ )
        line_ = &entity;
    if( line_ != &entity )
        return;

    Q3PopupMenu* limaMenu = new Q3PopupMenu( menu );
    for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
    {
        int id = limaMenu->insertItem( tools::ToString( (kernel::E_FuncLimaType)i ), this, SLOT( MenuItemValidated( int ) ) );
        limaMenu->setItemParameter( id, i );
        limaMenu->setItemChecked( id, functions_->isSelected( i ) );
    }
    menu.InsertItem( "Parameter", tools::translate( "LimaParameter", "Set '%1' function" ).arg( GetName() ), limaMenu );
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
// Name: LimaParameter::CommitTo
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    kernel::Lines lines;
    if( line_ )
    {
        GeometrySerializer serializer( lines, converter_ );
        line_->Get< kernel::Positions >().Accept( serializer );
    }
    std::auto_ptr< actions::parameters::Lima > param( new actions::parameters::Lima( kernel::OrderParameter( GetName().ascii(), "lima", false ), converter_, lines ) );
    for( unsigned int i = 0; i < functions_->count(); ++i )
        if( functions_->isSelected( i ) )
            param->AddFunction( i );
    schedule_->CommitTo( *param );
    parameter.AddParameter( *param.release() );
}
