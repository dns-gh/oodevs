// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UsagesDockWidget_h_
#define __UsagesDockWidget_h_

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

namespace gui
{
    class RichPushButton;
    template< typename T > class RichWidget;
    template< typename T > class RichWidget;
}

class StaticModel;

// =============================================================================
/** @class  UsagesDockWidget
    @brief  UsagesDockWidget
*/
// Created: ABR 2012-05-16
// =============================================================================
class UsagesDockWidget : public gui::RichDockWidget
                       , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
                       , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UsagesDockWidget( QWidget* parent, kernel::Controllers& controller, StaticModel& model );
    virtual ~UsagesDockWidget();
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifySelectionChanged( const T_Elements& elements );
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& element );
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& element );
    //@}

    //! @name Operations
    //@{
    void Clean();
    void Initialize();
    //@}

    //! @name Events
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* pEvent );
    //@}

private slots:
    //! @name Slots
    //@{
    void Add();
    void Delete();
    void OnItemValueChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void AddItem( const std::string& role, int value );
    void AddMotivation( const std::string& role, int value );
    void RemoveMotivation( const std::string& role );
    void Validate();
    void Load();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel& staticModel_;
    gui::RichWidget< QComboBox >* pMotivations_;
    gui::RichPushButton* pButton_;
    gui::RichWidget< QTableWidget >* pTable_;
    bool isEditing_;
    T_Elements selectedElements_;
    //@}
};

#endif // __UsagesDockWidget_h_
