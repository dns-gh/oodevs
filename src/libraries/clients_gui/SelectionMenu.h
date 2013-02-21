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
#include "Layer_ABC.h"

namespace kernel
{
    class GraphicalEntity_ABC;
}

namespace gui
{
    class ColorStrategy;
    class DrawingTypes;
    class GlTools_ABC;
    class GlWidget;
    class EntitySymbols;

// =============================================================================
/** @class  SelectionMenu
    @brief  SelectionMenu
*/
// Created: ABR 2013-01-30
// =============================================================================
class SelectionMenu : public QObject
                    , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SelectionMenu( EntitySymbols& entitySymbols, ColorStrategy& colorStrategy, DrawingTypes& drawingTypes, GlTools_ABC& tools );
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
    //@}

private:
    //! @name Helpers
    //@{
    bool GenerateIcons();
    QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, float r, float g, float b, const std::string& category = "", float markerPixelRatio = 1.f ) const;
    QPixmap SelectionMenu::ExtractDrawingSample( const std::string& code, const QColor& color, const std::string& category = "", float markerPixelRatio = 1.f ) const;
    //@}

private:
    //! @name Member data
    //@{
    EntitySymbols& entitySymbols_;
    ColorStrategy& colorStrategy_;
    DrawingTypes& drawingTypes_;
    GlTools_ABC& tools_;

    Layer_ABC::T_LayerElements extractedElements_;
    std::map< const kernel::GraphicalEntity_ABC*, QPixmap > icons_;
    std::auto_ptr< QMouseEvent > mouseEvent_;
    geometry::Point2f point_;
    gui::GlWidget* parent_;
    //@}
};

} //! namespace gui

#endif // __SelectionMenu_h_
