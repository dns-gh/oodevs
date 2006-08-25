// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_AgentsLayer_h_
#define __Gui_AgentsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class AgentsLayer : public QObject
                  , public EntityLayer< kernel::Agent_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    void Aggregate   ( const kernel::Agent_ABC& agent );
    void Disaggregate( const kernel::Agent_ABC& agent );
    //@}

private slots:
    //! @name Slots
    //@{
    void Aggregate   ();
    void Disaggregate();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC&, kernel::ContextMenu& );
    virtual void Select( const kernel::Entity_ABC&, bool );
    virtual void DisplayTooltip( const kernel::Agent_ABC& entity, kernel::Displayer_ABC& displayer );
    void Toggle( const kernel::Agent_ABC& automat, bool aggregate );
    //@}

    //! @name Types
    //@{
    typedef std::set< const kernel::Agent_ABC* > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    T_Agents aggregated_;
    //@}
};

}

#endif // __Gui_AgentsLayer_h_
