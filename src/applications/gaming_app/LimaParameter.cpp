// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LimaParameter.h"
#include "moc_LimaParameter.cpp"
#include "ParamDateTime.h"
#include "gaming/Lima.h"
#include "gaming/Tools.h"
#include "gaming/ActionParameterLima.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LimaParameter constructor
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
LimaParameter::LimaParameter( QObject* parent, const QString& name, const kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, const Lima& lima )
    : QObject( parent )
    , Param_ABC( name )
    , converter_( converter )
    , simulation_( simulation )
    , lima_( &lima )
    , schedule_( 0 )
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
    return lima_ && ( schedule_ && schedule_->CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::BuildInterface
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::BuildInterface( QWidget* parent )
{
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, GetName(), parent );
    new QLabel( tr( "Line" ), group );
    entityLabel_ = new QLabel( "---", group );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    new QLabel( tr( "Functions" ), group );
    functions_ = new QListBox( group );
    functions_->setSelectionMode( QListBox::Multi );
    for( unsigned int i = 0; i < eLimaFuncNbr; ++i )
        functions_->insertItem( tools::ToString( (E_FuncLimaType)i ), i );
    functions_->setRowMode( QListBox::FitToHeight );
    functions_->setFixedSize( 150, functions_->itemHeight( 0 ) * 4 );
    schedule_ = new ParamDateTime( this, tr( "Schedule" ), simulation_, true ); // $$$$ SBO 2007-05-14: optional
    schedule_->BuildInterface( group );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::Draw
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 1, 0, 0, 0.5f );
        lima_->Interface().Apply( &kernel::Drawable_ABC::Draw, point, viewport, tools ); // $$$$ SBO 2007-05-02:
        // $$$$ AGE 2007-05-09: pourquoi pas juste Draw ??
        glColor3f( 0.7f, 0, 0 );
        QStringList functions;
        for( unsigned int i = 0; i < functions_->count(); ++i )
            if( functions_->isSelected( i ) )
                functions.append( tools::ToString( (E_FuncLimaType)i ) );
        const geometry::Point2f position = lima_->Get< kernel::Positions >().GetPosition();
        const geometry::Vector2f lineFeed = geometry::Vector2f( 0, -18.f * tools.Pixels() ); // $$$$ SBO 2007-05-15: hard coded \n
        if( ! functions.isEmpty() )
            tools.Print( functions.join( ", " ).ascii(), position + lineFeed, QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
        schedule_->Draw( position + lineFeed * 2.f, viewport, tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::MenuItemValidated
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::MenuItemValidated( int index )
{
    entityLabel_->setText( lima_->GetName() );
    functions_->setSelected( index, !functions_->isSelected( index ) );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::NotifyContextMenu
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() || !lima_ || lima_ != &entity )
        return;
    QPopupMenu* limaMenu = new QPopupMenu( menu );
    for( unsigned int i = 0; i < eLimaFuncNbr; ++i )
    {
        int id = limaMenu->insertItem( tools::ToString( (E_FuncLimaType)i ), this, SLOT( MenuItemValidated( int ) ) );
        limaMenu->setItemParameter( id, i );
        limaMenu->setItemChecked( id, functions_->isSelected( i ) );
    }
    menu.InsertItem( "Parameter", tr( "Set '%1' function" ).arg( GetName() ), limaMenu );
}

// -----------------------------------------------------------------------------
// Name: LimaParameter::CommitTo
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void LimaParameter::CommitTo( ActionParameterContainer_ABC& parameter ) const
{
    kernel::Lines lines;
    lima_->CopyTo( lines );
    std::auto_ptr< ActionParameterLima > param( new ActionParameterLima( OrderParameter( GetName().ascii(), "lima", false ), converter_, lines ) );
    for( unsigned int i = 0; i < functions_->count(); ++i )
        if( functions_->isSelected( i ) )
            param->AddFunction( i );
    schedule_->CommitTo( *param );
    parameter.AddParameter( *param.release() );
}
