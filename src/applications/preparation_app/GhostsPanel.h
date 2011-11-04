// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostsPanel_h_
#define __GhostsPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/SymbolVisitor_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ModelLoaded;
    class SymbolFactory;
    class SymbolRule;
}

namespace gui
{
    class ColorStrategy_ABC;
    class NatureEditionWidget;
    class PanelStack_ABC;
    class SymbolIcons;

    class UnitPreviewIcon;
    class NatureEditionWidget;
}

class GhostSymbolEditor;

// =============================================================================
/** @class  GhostsPanel
    @brief  GhostsPanel
*/
// Created: ABR 2011-10-12
// =============================================================================
class GhostsPanel : public gui::InfoPanel_ABC
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GhostsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers,
                          kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~GhostsPanel();
    //@}

    //! @name Operations
    //@{
    void Load();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private slots:
    //! @name Helpers
    //@{
    void IconDragged();
    void UpdateWarning();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&   controllers_;
    kernel::SymbolFactory& symbolsFactory_;
    kernel::GhostPrototype protoType_;
    // Name and type
    QLineEdit*             nameLineEdit_;
    QLineEdit*             typeLineEdit_;
    // Symbol
    GhostSymbolEditor*     symbolEditor_;
    // Warning
    QLabel*                warningLabel_;
    //@}
};

#endif // __GhostsPanel_h_
