// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_TOOLS_H
#define CLIENTS_GUI_TOOLS_H

#include <geometry/Types.h>

namespace kernel
{
    class Entity_ABC;
    class Profile_ABC;
    class OptionsController;
}

namespace gui
{
    class ColorButton;
    class GlTools_ABC;
}

class QVBoxLayout;
class QStandardItemModel;

namespace tools
{
    class Path;

    bool IsPropagationDir( const tools::Path& dir );
    bool IsCommandPost( const kernel::Entity_ABC& entity );
    bool IsEngaged( const kernel::Entity_ABC& entity );
    bool IsSuperiorEngaged( const kernel::Entity_ABC& entity );
    bool LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool CanOneSubordinateBeOrdered( const kernel::Profile_ABC& profile, const kernel::Entity_ABC& entity );
    tools::Path SanitizeFileName( QString text, const QString& after = "-" );
    void SetKarma( const kernel::Entity_ABC& entity, std::string& symbol );
    void SetLevel( const kernel::Entity_ABC& entity, std::string& level );
    QImage DrawText( const QString& text, const QFont& font, const QColor& color );
    void DrawPickingText( const QString& text, const QFont& font, const geometry::Point2f& point, const gui::GlTools_ABC& tools );

    gui::ColorButton* AddColorButton( QVBoxLayout* mainLayout,
                                      kernel::OptionsController& options,
                                      const QString& objectName,
                                      const QString& name,
                                      const std::string& optionName,
                                      const QColor& defaultColor );

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
}  //! namespace tools

#endif  // CLIENTS_GUI_TOOLS_H
