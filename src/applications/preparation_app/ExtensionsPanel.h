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
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Team_ABC;
}

// =============================================================================
/** @class  ExtensionsPanel
    @brief  ExtensionsPanel
*/
// Created: JSR 2011-12-08
// =============================================================================
class ExtensionsPanel : public gui::ExtensionsPanel
                      , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions,
                              const tools::Resolver< kernel::Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations );
    virtual ~ExtensionsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    virtual void Load();
    //@}

private:
    //! @name From ContextMenuObserver_ABC
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    void DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void OnDeleteCPExtensions();
    virtual void OnFillEmptyCPExtensions();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OnChangeNationality();
    virtual void OnChangeTypeSIOC();
    virtual void OnChangeIdCDB( const std::string& idCDB );
    virtual void SetAllIdCDB( const QStringList& IdCDB );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* cpSuperior_;
    std::vector< std::string > allIdCDB_;
    std::map< std::string, unsigned int > usedIdCDB_;
    const tools::Resolver< kernel::Agent_ABC >& agents_;
    int idIndex_;
    //@}
};

#endif // __ExtensionsPanel_h_
