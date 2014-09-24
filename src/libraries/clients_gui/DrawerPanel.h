// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerPanel_h_
#define __DrawerPanel_h_

#include "InfoPanel_ABC.h"
#include "Drawing.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelLoaded;
    class Drawing_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
    class ColorButton;
    class DrawerModel;
    class Drawing;
    class DrawingCategory;
    class DrawingCategoryItem;
    class DrawingTemplate;
    class DrawingTypes;
    class ParametersLayer;

// =============================================================================
/** @class  DrawerPanel
    @brief  Drawer panel
*/
// Created: AGE 2006-09-01
// =============================================================================
class DrawerPanel : public InfoPanel_ABC
                  , public tools::Observer_ABC
                  , public tools::SelectionObserver_ABC
                  , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                  , public tools::ElementObserver_ABC< DrawingCategory >
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , public tools::SelectionObserver_Base< kernel::Drawing_ABC >
                  , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                  , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                  , private ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DrawerPanel( QWidget* parent,
                          PanelStack_ABC& panel,
                          const std::shared_ptr< ParametersLayer >& layer,
                          kernel::Controllers& controllers,
                          DrawerModel& model,
                          DrawingTypes& types,
                          const tools::ExerciseConfig& config );
    virtual ~DrawerPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void setEnabled( bool );
    void Open();
    void Save();
    void Clear();
    void OnSelect( const DrawingTemplate& style );
    void OnColorChange( const QColor& color );
    void StartDrawing();
    void StartTextEdition();
    void OnLineChanged( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const DrawingCategory& category );
    virtual void NotifyDeleted( const DrawingCategory& category );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Drawing_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );

    virtual void Handle( kernel::Location_ABC& location );
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const DrawingCategory*, DrawingCategoryItem* > T_CategoryItems;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::shared_ptr< ParametersLayer > layer_;
    DrawerModel& model_;
    DrawingTypes& types_;
    const tools::ExerciseConfig& config_;
    ColorButton* color_;
    QLabel* parentLabel_;
    QToolBox* toolBox_;
    const DrawingTemplate* selectedStyle_;
    kernel::SafePointer< Drawing > selectedDrawing_;
    T_CategoryItems categories_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    E_Dash_style dashStyle_;
    //@}
};

}

#endif // __DrawerPanel_h_
