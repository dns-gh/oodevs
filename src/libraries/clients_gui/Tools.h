// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_TOOLS_H
#define CLIENTS_GUI_TOOLS_H

#include <geometry/Types.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
    class OptionsController;
}

namespace gui
{
    class OptionColorButton;
    class GLView_ABC;
}

class QMenu;
class QObject;
class QStandardItemModel;
class QVBoxLayout;
class QWidget;

namespace tools
{
    class Path;

    bool IsPropagationDir( const tools::Path& dir );
    bool IsCommandPost( const kernel::Entity_ABC& entity );
    bool IsEngaged( const kernel::Entity_ABC& entity );
    bool IsSuperiorEngaged( const kernel::Entity_ABC& entity );
    bool LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool HasSubordinate( const kernel::Entity_ABC& entity, const std::function< bool( const kernel::Entity_ABC& ) >& fun );
    bool CanOneSubordinateBeOrdered( const kernel::Profile_ABC& profile, const kernel::Entity_ABC& entity );
    tools::Path SanitizeFileName( QString text, const QString& after = "-" );
    void SetKarma( const kernel::Entity_ABC& entity, std::string& symbol );
    void SetLevel( const kernel::Entity_ABC& entity, std::string& level );
    QImage DrawText( const QString& text, const QFont& font, const QColor& color );
    void DrawPickingText( const QString& text, const QFont& font, const geometry::Point2f& point, const gui::GLView_ABC& tools );

    QWidget* AddGroupBoxWidget( const QString& title, const QString& objectName, QWidget* widget, QWidget* parent = 0 );
    QWidget* AddLabeledWidget( const QString& label, QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::AlignLeft, QWidget* parent = 0 );

    template< typename T >
    void UpdateEntityNameInModel( QStandardItemModel& model,
                                  const kernel::Entity_ABC& entity,
                                  int role )
    {
        for( int i = 0; i < model.rowCount(); ++i )
            if( auto item = model.item( i, 0 ) )
                if( auto itemEntity = item->data( role ).value< const T* >() )
                    if( itemEntity == &entity )
                        item->setText( itemEntity->GetName() );
    }
    template< typename T >
    void UpdateEntityNameInKnowledgeModel( QStandardItemModel& model,
                                           const kernel::Entity_ABC& entity,
                                           int role )
    {
        for( int i = 0; i < model.rowCount(); ++i )
            if( auto item = model.item( i, 0 ) )
                if( auto itemEntity = item->data( role ).value< const T* >() )
                    if( itemEntity->GetEntity() == &entity )
                        item->setText( itemEntity->GetName() );
    }

    QString Point2fToString( const geometry::Point2f& point );
    QString Point3fToString( const geometry::Point3f& point );
    QString Vector3fToString( const geometry::Vector3f& vector );
    geometry::Point2f StringToPoint2f( const QString& point );
    geometry::Point3f StringToPoint3f( const QString& point );
    geometry::Vector3f StringToVector3f( const QString& vector );

    QMenu* CreateWindowMenu( kernel::Controllers& controllers, QWidget& parent, QObject& glWidgetManager );

}  //! namespace tools

#endif  // CLIENTS_GUI_TOOLS_H
