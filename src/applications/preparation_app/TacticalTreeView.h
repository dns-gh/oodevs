// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TacticalTreeView_h_
#define __TacticalTreeView_h_

#include "clients_gui/TacticalTreeView.h"

class Model;

// =============================================================================
/** @class  TacticalTreeView
    @brief  TacticalTreeView
*/
// Created: JSR 2012-09-07
// =============================================================================
class TacticalTreeView : public gui::TacticalTreeView
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, Model& model );
    virtual ~TacticalTreeView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target);
    virtual void Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::AgentType& item, kernel::Entity_ABC& target );
    virtual void Drop( const kernel::AutomatType& item, kernel::Entity_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    //@}
};

#endif // __TacticalTreeView_h_
