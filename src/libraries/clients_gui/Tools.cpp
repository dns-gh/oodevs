// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Tools.h"
#include "AutomatDecisions.h"
#include "ColorButton.h"
#include "SignalAdapter.h"

#include "clients_gui/GLView_ABC.h"
#include "clients_gui/MergingTacticalHierarchies.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/assign.hpp>

#include <tools/Path.h>

bool tools::IsPropagationDir( const tools::Path& dir )
{
    return dir.IsDirectory() && ( dir / "propagation.xml" ).Exists();
}

bool tools::IsCommandPost( const kernel::Entity_ABC& entity )
{
    if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
        return pAttributes->IsCommandPost();
    return false;
}

bool tools::IsEngaged( const kernel::Entity_ABC& entity )
{
    if( const gui::AutomatDecisions* decisions =
        dynamic_cast< const gui::AutomatDecisions* >( entity.Retrieve< gui::Decisions_ABC >() ) )
        return decisions->IsEngaged();
    return false;
}

bool tools::IsSuperiorEngaged( const kernel::Entity_ABC& entity )
{
    if( const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior() )
        return tools::IsEngaged( *superior );
    return false;
}

bool tools::LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
{
    return entity1.GetId() < entity2.GetId();
}

bool tools::LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
{
    if( IsCommandPost( entity1 ) )
        return true;
    if( IsCommandPost( entity2 ) )
        return false;
    return LessThanById( entity1, entity2 );
}

tools::Path tools::SanitizeFileName( QString text, const QString& after /*= "-"*/ )
{
    text.replace( QRegExp( "[/\"<>|*\?:\\\\]+" ), after );
    return tools::Path::FromUnicode( text.simplified().toStdWString() );
}

void tools::SetKarma( const kernel::Entity_ABC& entity, std::string& symbol )
{
    if( const kernel::TacticalHierarchies* pHierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
        kernel::App6Symbol::SetKarma( symbol, pHierarchy->GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

void tools::SetLevel( const kernel::Entity_ABC& entity, std::string& level )
{
    if( const kernel::TacticalHierarchies* pHierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        std::string levelSymbol = pHierarchy->GetLevel();
        if( !levelSymbol.empty() )
        {
            level = gui::MergingTacticalHierarchies::DecreaseLevel( levelSymbol );
            if( level.find( "levels/" ) != std::string::npos )
                level.erase( 0, 7 );
        }
    }
}

bool tools::CanOneSubordinateBeOrdered( const kernel::Profile_ABC& profile, const kernel::Entity_ABC& entity )
{
    auto it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    if( profile.CanBeOrdered( entity ) )
        return true;
    while( it.HasMoreElements() )
        if( CanOneSubordinateBeOrdered( profile, it.NextElement() ) )
            return true;
    return false;
}

QImage tools::DrawText( const QString& text, const QFont& font, const QColor& color )
{
    if( text.isEmpty() )
        return QImage();

    const QFontMetrics metrics( font );
    QStringList list = QStringList::split( '\n', text, true );
    QPixmap pix( metrics.boundingRect( text ).width(), metrics.height() * list.size() );
    pix.fill( Qt::transparent );
    QPainter painter( &pix );
    painter.setPen( color );
    painter.setFont( font );
    painter.drawText( QRectF( 0, 0, pix.width(), pix.height() ), Qt::AlignVCenter, text );
    return pix.convertToImage();
}

void tools::DrawPickingText( const QString& text, const QFont& font, const geometry::Point2f& point, const gui::GLView_ABC& tools )
{
    const QFontMetrics metrics( font );
    QStringList list = QStringList::split( '\n', text, true );
    const auto height = metrics.height() * list.size();
    const auto width = metrics.boundingRect( text ).width();
    const geometry::Rectangle2f box( geometry::Point2f( point.X(), point.Y() - height * tools.Pixels() ),
                                     geometry::Point2f( point.X() + width * tools.Pixels(), point.Y() ) );
    const T_PointVector polygon = boost::assign::list_of( geometry::Point2f( box.Left(), box.Top() ) )
                                                     ( geometry::Point2f( box.Right(), box.Top() ) )
                                                     ( geometry::Point2f( box.Right(), box.Bottom() ) )
                                                     ( geometry::Point2f( box.Left(), box.Bottom() ) )
                                                     ( geometry::Point2f( box.Left(), box.Top() ) );
    tools.DrawPolygon( polygon );
}

QWidget* tools::AddGroupBoxWidget( const QString& title,
                                   const QString& objectName,
                                   QWidget* widget,
                                   QWidget* parent /* = 0 */ )
{
    QGroupBox* result = new QGroupBox( parent );
    result->setObjectName( objectName );
    result->setTitle( title );
    QVBoxLayout* layout = new QVBoxLayout( result );
    layout->addWidget( widget );
    return result;
}

QWidget* tools::AddLabeledWidget( const QString& label,
                                  QWidget* widget,
                                  int stretch /* = 0 */,
                                  Qt::Alignment alignment /* = Qt::AlignLeft */,
                                  QWidget* parent /* = 0 */ )
{
    QWidget* result = new QWidget( parent );
    QHBoxLayout* layout = new QHBoxLayout( result );
    layout->setMargin( 0 );
    layout->addWidget( new QLabel( label ), 0, Qt::AlignLeft );
    layout->addWidget( widget, stretch, alignment );
    return result;
}

bool tools::HasSubordinate( const kernel::Entity_ABC& entity, const std::function< bool( const kernel::Entity_ABC& ) >& fun )
{
    bool children = false;
    auto it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        if( !fun( child ) )
            return false;
        children = true;
    }
    return children;
}
