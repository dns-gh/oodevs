// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerPanel_h_
#define __DrawerPanel_h_

#include "InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

class QToolBox;

namespace kernel {
    class GlTools_ABC;
    class Controllers;
    class ModelLoaded;
}

namespace svg {
    class TextRenderer;
}

namespace gui
{
    class ParametersLayer;
    class DrawingTemplate;
    class DrawingCategory;
    class ColorButton;
    class DrawerModel;
    class Drawing_ABC;

// =============================================================================
/** @class  DrawerPanel
    @brief  Drawer panel
*/
// Created: AGE 2006-09-01
// =============================================================================
class DrawerPanel : public InfoPanel_ABC
                  , public tools::Observer_ABC
                  , public kernel::ElementObserver_ABC< DrawingCategory >
                  , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                  , public tools::SelectionObserver< Drawing_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawerPanel( QWidget* parent, PanelStack_ABC& panel, ParametersLayer& layer, kernel::Controllers& controllers, DrawerModel& model );
    virtual ~DrawerPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void Open();
    void Save();
    void Clear();
    void OnSelect( const DrawingTemplate& style );
    void OnColorChange( const QColor& color );
    void StartDrawing();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerPanel( const DrawerPanel& );            //!< Copy constructor
    DrawerPanel& operator=( const DrawerPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const DrawingCategory& category );
    virtual void NotifyDeleted( const DrawingCategory& category );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifySelected( const Drawing_ABC* drawing );
    //@}

    //! @name Types
    //@{
    typedef std::map< const DrawingCategory*, QWidget* > T_CategoryItems;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ParametersLayer& layer_;
    DrawerModel& model_;

    ColorButton* color_;
    QToolBox* toolBox_;
    const DrawingTemplate* selectedStyle_;
    kernel::SafePointer< Drawing_ABC > selectedDrawing_;
    T_CategoryItems categories_;
    //@}
};

}

#endif // __DrawerPanel_h_
