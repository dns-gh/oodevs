// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationsLayer_h_
#define __PopulationsLayer_h_

#include "clients_gui/PopulationsLayer.h"
#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

class Model;

// =============================================================================
/** @class  PopulationsLayer
    @brief  PopulationsLayer
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationsLayer : public gui::PopulationsLayer
                       , public kernel::SelectionObserver_Base< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile );
    virtual ~PopulationsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationsLayer( const PopulationsLayer& );            //!< Copy constructor
    PopulationsLayer& operator=( const PopulationsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Entity_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    kernel::SafePointer< kernel::Population_ABC > selectedPopulation_;
    //@}
};

#endif // __PopulationsLayer_h_
