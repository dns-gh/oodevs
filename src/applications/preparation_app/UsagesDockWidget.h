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

namespace kernel
{
    class Controllers;
}

class StaticModel;

// =============================================================================
/** @class  UsagesDockWidget
    @brief  UsagesDockWidget
*/
// Created: ABR 2012-05-16
// =============================================================================
class UsagesDockWidget : public gui::RichDockWidget
                       //, public MultipleSelectionObserver_ABC
                       //, public tools::ElementObserver_ABC< urban::TerrainObject_ABC >
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
    //virtual void SelectionChanged();
    //virtual void NotifyUpdated( const urban::TerrainObject_ABC& element );
    //virtual void NotifyDeleted( const urban::TerrainObject_ABC& element );
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
    void Load(); // $$$$ ABR 2012-05-16: Never used ?
    //@}

private:
    //! @name Helpers
    //@{
    void AddItem( const std::string& name, int value );
    void AddMotivation( const std::string& name, int value );
    void RemoveMotivation( const std::string& name );
    void Validate();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, float > T_Motivations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel&         staticModel_;
    QComboBox*           pMotivations_;
    QPushButton*         pButton_;
    QTableWidget*        pTable_;
    bool                 isEditing_;
    //@}
};

#endif // __UsagesDockWidget_h_
