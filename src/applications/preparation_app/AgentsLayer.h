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
                  , public kernel::MultipleSelectionObserver_Base< kernel::Automat_ABC >
                  , public kernel::MultipleSelectionObserver_Base< kernel::Formation_ABC >
                  , public kernel::MultipleSelectionObserver_Base< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const kernel::Profile_ABC& profile );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void MultipleSelect( const std::vector< const kernel::Agent_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Team_ABC* >& elements );
    virtual void AfterSelection();

    bool IsValidTemplate( QDropEvent* event ) const;
    bool IsEligibleForDrag( const geometry::Point2f& point ) const;

    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
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
    geometry::Point2f                            draggingPoint_;
    geometry::Point2f                            draggingOffset_;
    geometry::Point2f                            oldPosition_;
    std::auto_ptr< QWidget > dummy_;
    //@}
};

#endif // __AgentsLayer_h_
