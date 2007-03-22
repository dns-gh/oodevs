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

#include "clients_kernel/ElementObserver_ABC.h"

class QToolBox;

namespace xml {
    class xistream;
}

namespace kernel {
    class GlTools_ABC;
    class Controllers;
}

namespace svg {
    class TextRenderer;
}

namespace gui
{
    class DrawerLayer;
    class DrawerStyle;
    class DrawerCategory;
    class ColorButton;
    class DrawerFactory;
    class DrawerModel;

// =============================================================================
/** @class  DrawerPanel
    @brief  Drawer panel
*/
// Created: AGE 2006-09-01
// =============================================================================
class DrawerPanel : public QVBox
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< DrawerCategory >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawerPanel( QWidget* parent, DrawerLayer& layer, kernel::GlTools_ABC& tools, kernel::Controllers& controllers );
    virtual ~DrawerPanel();
    //@}

    //! @name Operations
    //@{
    //@}

signals:
    //! @name Signals
    //@{
    void Closed();
    //@}

private slots:
    //! @name Slots
    //@{
    void Open();
    void Save();
    void Clear();
    void OnSelect( DrawerStyle& style );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerPanel( const DrawerPanel& );            //!< Copy constructor
    DrawerPanel& operator=( const DrawerPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const DrawerCategory& category );
    virtual void hideEvent( QHideEvent* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    DrawerLayer& layer_;
    svg::TextRenderer* renderer_;
    DrawerFactory& factory_;
    DrawerModel& model_;

    ColorButton* color_;
    QToolBox* toolBox_;
    //@}
};

}

#endif // __DrawerPanel_h_
