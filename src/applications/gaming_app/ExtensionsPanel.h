// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionsPanel_h_
#define __ExtensionsPanel_h_

#include "clients_gui/ExtensionsPanel.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class AgentTypes;
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class EntitySymbols;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ExtensionsPanel
    @brief  ExtensionsPanel
*/
// Created: ABR 2011-05-12
// =============================================================================
class ExtensionsPanel : public gui::ExtensionsPanel
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const Model& model,
                              const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~ExtensionsPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValidate();
    void OnReset();
    virtual void Commit();
    virtual void OnActivationChanged( bool );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void SetReadOnly( bool readOnly ) const;
    void UpdateBackUp();
    void Reset();
    void ChangeButtonsState( bool state );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel&               actions_;
    kernel::AgentTypes&                  types_;
    const kernel::Time_ABC&              simulation_;
    bool                                 hasChanged_;
    QPushButton*                         validateButton_;
    QPushButton*                         resetButton_;
    std::map< std::string, std::string > backup_;
    bool                                 activatedBackup_;
    //@}
};

#endif // __ExtensionsPanel_h_
