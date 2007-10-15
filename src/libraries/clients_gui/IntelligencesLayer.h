// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_IntelligencesLayer_h_
#define __gui_IntelligencesLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Intelligence_ABC.h"

namespace kernel
{
    class IntelligenceFactory_ABC;
}

namespace gui
{

// =============================================================================
/** @class  IntelligencesLayer
    @brief  IntelligencesLayer
*/
// Created: SBO 2007-10-12
// =============================================================================
class IntelligencesLayer : public EntityLayer< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, kernel::IntelligenceFactory_ABC& factory );
    virtual ~IntelligencesLayer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesLayer( const IntelligencesLayer& );            //!< Copy constructor
    IntelligencesLayer& operator=( const IntelligencesLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Intelligence_ABC* element );

    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    bool IsEligibleForDrag( const geometry::Point2f& point ) const;
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual void Move( kernel::Positions& position, const geometry::Point2f& point ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    kernel::IntelligenceFactory_ABC& factory_;
    kernel::SafePointer< kernel::Intelligence_ABC > selectedIntelligence_;
    //@}
};

}

#endif // __gui_IntelligencesLayer_h_
