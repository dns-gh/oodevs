// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsLayer_h_
#define __AgentsLayer_h_

#include "clients_gui/AgentsLayer.h"
#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class KnowledgeGroup_ABC;
}

class Model;

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: SBO 2006-08-31
// =============================================================================
class AgentsLayer : public gui::AgentsLayer
                  , public kernel::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::SafePointer< kernel::Agent_ABC > selectedAgent_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedGroup_;
    //@}
};

#endif // __AgentsLayer_h_
