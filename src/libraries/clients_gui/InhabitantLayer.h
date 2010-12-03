// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantLayer_h_
#define __InhabitantLayer_h_

#include "Layer_ABC.h"
#include "EntityLayer.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class View_ABC;

// =============================================================================
/** @class  InhabitantLayer
@brief  InhabitantLayer
*/
// Created: SLG 2006-03-23
// =============================================================================
class InhabitantLayer : public Layer_ABC
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Inhabitant_ABC >
                      , public kernel::ActivationObserver_ABC< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~InhabitantLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual void Reset2d();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Inhabitant_ABC* object );
    virtual void NotifyActivated( const kernel::Inhabitant_ABC& element );
    virtual void NotifyDeleted( const kernel::Inhabitant_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Inhabitant_ABC& element );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantLayer( const InhabitantLayer& );            //!< Copy constructor
    InhabitantLayer& operator=( const InhabitantLayer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    View_ABC& view_;
    const kernel::Inhabitant_ABC*   selectedInhab_;
    //@}
};

}
#endif // __InhabitantLayer_h_
