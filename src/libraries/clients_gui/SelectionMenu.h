// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SelectionMenu_h_
#define __SelectionMenu_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/OptionsObserver_ABC.h"
#include "Layer_ABC.h"

namespace kernel
{
    class GraphicalEntity_ABC;
    class Controllers;
}

namespace gui
{
    class ColorStrategy;
    class DrawingTypes;
    class GlTools_ABC;
    class GlWidget;
    class Gl3dWidget;
    class EntitySymbols;

// =============================================================================
/** @class  SelectionMenu
    @brief  SelectionMenu
*/
// Created: ABR 2013-01-30
// =============================================================================
class SelectionMenu : public QObject
                    , public tools::Observer_ABC
                    , public kernel::OptionsObserver_ABC
                    , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SelectionMenu( kernel::Controllers& controllers, EntitySymbols& entitySymbols, ColorStrategy& colorStrategy,
                            DrawingTypes& drawingTypes, GlTools_ABC& tools );
    virtual ~SelectionMenu();
    //@}

    //! @name Operations
    //@{
    void ExecMenu( const Layer_ABC::T_LayerElements& extractedElements, const geometry::Point2f& point,
                   const QPoint &globalPos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers );
    //@}

private slots:
    //! @name Slots
    //@{
    void GenerateMenu();
    void OnWidget2dChanged( gui::GlWidget* );
    void OnWidget3dChanged( gui::Gl3dWidget* );
    void OnSelectionChanged( QAction* action );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< Layer_ABC*, const kernel::GraphicalEntity_ABC* > T_Result;
    //@}

private:
    //! @name Helpers
    //@{
    bool GenerateIcons();
    void FilterElement( const Layer_ABC::T_LayerElements& extractedElements );
    QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, float r, float g, float b, const std::string& category = "", float markerPixelRatio = 1.f ) const;
    QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, const QColor& color, const std::string& category = "", float markerPixelRatio = 1.f ) const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    T_Result GetSelected( const QString& text );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    EntitySymbols& entitySymbols_;
    ColorStrategy& colorStrategy_;
    DrawingTypes& drawingTypes_;
    GlTools_ABC& tools_;
    geometry::Point2f point_;
    Layer_ABC::T_LayerElements extractedElements_;
    std::map< const kernel::GraphicalEntity_ABC*, QPixmap > icons_;
    std::unique_ptr< QMouseEvent > mouseEvent_;
    gui::GlWidget* parent2d_;
    gui::Gl3dWidget* parent3d_;
    unsigned int moreElements_;
    bool mode3d_;
    QAction* current_;
    //@}
};

} //! namespace gui

#endif // __SelectionMenu_h_
