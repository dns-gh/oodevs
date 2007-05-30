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
    class Automat_ABC;
    class Formation_ABC;
    class Team_ABC;
}

class Model;
class ModelBuilder;

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: SBO 2006-08-31
// =============================================================================
class AgentsLayer : public gui::AgentsLayer
                  , public kernel::SelectionObserver_Base< kernel::Automat_ABC >
                  , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
                  , public kernel::SelectionObserver_Base< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const kernel::Profile_ABC& profile );
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
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Team_ABC& element );

    bool IsValidTemplate( QDragEnterEvent* event ) const;
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    ModelBuilder& modelBuilder_;
    kernel::SafePointer< kernel::Agent_ABC >     selectedAgent_;
    kernel::SafePointer< kernel::Automat_ABC >   selectedAutomat_;
    kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    kernel::SafePointer< kernel::Team_ABC >      selectedTeam_;
    //@}
};

#endif // __AgentsLayer_h_
