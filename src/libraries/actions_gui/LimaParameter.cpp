// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "LimaParameter.h"
#include "moc_LimaParameter.cpp"
#include "ParamDateTime.h"
#include "ListParameter.h"
#include "actions/Lima.h"
#include "clients_gui/GlTools_ABC.h"
#include "actions_gui/MissionInterface_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: LimaParameter constructor
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
LimaParameter::LimaParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , controller_( builder.GetControllers().controller_ )
    , converter_   ( builder.GetStaticModel().coordinateConverter_ )
    , clickedLine_ ( 0 )
    , selectedLine_( 0 )
    , functions_   ( new QListWidget() )
    , schedule_    ( static_cast< ParamDateTime* >( &builder.BuildOne( kernel::OrderParameter( tools::translate( "LimaParameter", "Schedule" ).toStdString(), "datetime", true ), false ) ) )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter destructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
LimaParameter::~LimaParameter()
{
    controller_.Unregister( *this );
    delete functions_;
    delete schedule_;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::InternalCheckValidity
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
bool LimaParameter::InternalCheckValidity() const
{
    return selectedLine_ && schedule_ && schedule_->CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::BuildInterface
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
QWidget* LimaParameter::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QGridLayout* layout = new QGridLayout( group_ );

    entityLabel_ = new QLabel( "---", parent );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    functions_->setSelectionMode( QListWidget::MultiSelection );
    for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
        functions_->insertItem( i, tools::ToShortString( (kernel::E_FuncLimaType)i ) );

    QWidget* scheduleBox = schedule_->BuildInterface( parent );
    layout->addWidget( new QLabel( tools::translate( "LimaParameter", "Line" ), parent ), 0, 0 );
    layout->addWidget( entityLabel_, 0, 1 );
    layout->addWidget( new QLabel( tools::translate( "LimaParameter", "Functions" ), parent ), 1, 0 );
    layout->addWidget( functions_, 1, 1 );
    layout->addWidget( scheduleBox, 2, 0, 1, 2 );

    return group_;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::Draw
// Created: SBO 2007-05-02
// Modified: MGD 2010-10-27
// -----------------------------------------------------------------------------
void LimaParameter::Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( selectedLine_ )
    {
        glPushAttrib( GL_CURRENT_BIT );
            glColor4f( 1, 0, 0, 0.5f );
            selectedLine_->GetInterfaces().Apply( &::gui::Drawable_ABC::Draw, point, viewport, tools ); // $$$$ SBO 2007-05-02:
            // $$$$ AGE 2007-05-09: pourquoi pas juste Draw ??
            glColor3f( 0.7f, 0, 0 );
            QStringList functions;
            for( int i = 0; i < functions_->count(); ++i )
                if( functions_->isItemSelected( functions_->item( i ) ) )
                    functions.append( tools::ToShortString( (kernel::E_FuncLimaType)i ) );
            const geometry::Point2f position = selectedLine_->Get< kernel::Positions >().GetPosition();
            const geometry::Vector2f lineFeed = geometry::Vector2f( 0, -18.f * tools.Pixels() ); // $$$$ SBO 2007-05-15: hard coded \n
            if( ! functions.isEmpty() )
                tools.Print( functions.join( ", " ).toStdString(), position + lineFeed, QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
            schedule_->Draw( position + lineFeed * 2.f, viewport, tools );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::NotifyContextMenu
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() )
        return;
    clickedLine_ = &entity;
    Param_ABC::CreateMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::NotifyDeleted
// Created: LDC 2012-10-17
// -----------------------------------------------------------------------------
void LimaParameter::NotifyDeleted( const kernel::TacticalLine_ABC& entity )
{
    if( &entity == clickedLine_ )
        clickedLine_ = 0;
    if( &entity == selectedLine_ )
        selectedLine_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::CreateInternalMenu
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void LimaParameter::CreateInternalMenu( kernel::ContextMenu& menu )
{
    kernel::ContextMenu* internalMenu = new kernel::ContextMenu( &menu );
    internalMenu->setTitle( GetMenuName() );

    actions_.clear();
    for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
    {
        QAction* action = internalMenu->InsertItem( "", tools::ToString( (kernel::E_FuncLimaType)i ), i );
        action->setCheckable( true );
        action->setChecked( functions_->isItemSelected( functions_->item( i ) ) );
        actions_.push_back( action );
    }
    QObject::connect( internalMenu, SIGNAL( triggered( QAction* ) ), this, SLOT( OnMenuClick( QAction* ) ) );
    internalMenu_ = internalMenu;
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::OnMenuClick
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void LimaParameter::OnMenuClick( QAction* action )
{
    selectedLine_ = clickedLine_;
    clickedLine_ = 0;
    int index = 0;
    if( selectedLine_ )
    {
        for( auto it = actions_.begin(); it != actions_.end(); ++it, ++index )
            if( *it == action )
            {
                entityLabel_->setText( selectedLine_->GetName() );
                functions_->setItemSelected( functions_->item( index ), !functions_->isItemSelected( functions_->item( index ) ) );
            }
    }
    else
    {
        entityLabel_->setText( "---" );
        for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
            functions_->setItemSelected( functions_->item( i ), !functions_->isItemSelected( functions_->item( i ) ) );
    }
    if( group_ && IsOptional() )
        group_->setChecked( true );
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
            for( auto it = points.begin(); it != points.end(); ++it )
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
    if( IsChecked() && selectedLine_ )
    {
        kernel::Lines lines;
        GeometrySerializer serializer( lines, converter_ );
        selectedLine_->Get< kernel::Positions >().Accept( serializer );
        std::auto_ptr< actions::parameters::Lima > param( new actions::parameters::Lima( kernel::OrderParameter( GetName().toStdString(), "phaseline", false ), converter_, lines ) );
        for( int i = 0; i < functions_->count(); ++i )
            if( functions_->isItemSelected( functions_->item( i ) ) )
                param->AddFunction( i );
        schedule_->CommitTo( *param );
        parameter.AddParameter( *param.release() );
    }
    else
        parameter.AddParameter( *new actions::parameters::Lima( kernel::OrderParameter( GetName().toStdString(), "phaseline", false ) ) );
}
