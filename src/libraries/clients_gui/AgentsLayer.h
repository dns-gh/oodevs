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

namespace kernel
{
    class Automat_ABC;
}

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
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    void Aggregate   ( const kernel::Automat_ABC& automat );
    void Disaggregate( const kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Toggle( const kernel::Automat_ABC& automat, bool aggregate );
    virtual void Select     ( const kernel::Entity_ABC&, bool );
    //@}
};

}

#endif // __Gui_AgentsLayer_h_
