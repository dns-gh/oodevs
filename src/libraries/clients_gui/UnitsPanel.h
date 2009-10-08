// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitsPanel_h_
#define __UnitsPanel_h_

#include "InfoPanel_ABC.h"
#include "ValuedComboBox.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class AgentTypes;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
    class UnitListView;
    class UnitPreviewIcon;
    class SymbolIcons;
    class ColorStrategy_ABC;

// =============================================================================
/** @class  UnitsPanel
    @brief  UnitsPanel
*/
// Created: SBO 2006-08-28
// =============================================================================
class UnitsPanel : public InfoPanel_ABC
                 , public tools::Observer_ABC
                 , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UnitsPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::AgentTypes& agentTypes, ItemFactory_ABC& factory, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy );
    virtual ~UnitsPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void Sort();
    void OpenList();
    void CloseList();
    void SelectionChanged( QListViewItem* item );
    void IconDragged();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UnitsPanel( const UnitsPanel& );            //!< Copy constructor
    UnitsPanel& operator=( const UnitsPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Data members
    //@{
    kernel::Controllers& controllers_;
    ValuedComboBox< std::string >* combo_;
    UnitListView* list_;
    UnitPreviewIcon* icon_;
    //@}
};

}

#endif // __UnitsPanel_h_
