// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Drawing_h_
#define __Drawing_h_

#include "clients_gui/DrawerShape.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ServerPublisher_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class DrawingTypes;
}

namespace sword
{
    class CoordLatLongList;
}

class Publisher_ABC;

// =============================================================================
/** @class  Drawing
    @brief  Drawing
*/
// Created: SBO 2008-06-04
// =============================================================================
class Drawing : public gui::DrawerShape
              , public kernel::Updatable_ABC< sword::ShapeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Drawing( kernel::Controllers& controllers, const sword::ShapeCreation& asn, const kernel::Entity_ABC* entity,
                      const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher,
                      const kernel::CoordinateConverter_ABC& converter );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ShapeUpdate& message );
    virtual void Rename( const QString& name );
    virtual void NotifyDestruction() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Update();
    //@}

    //! @name Helpers
    //@{
    void SetLocation( const sword::CoordLatLongList& list );
    void SetText( const sword::Shape& shape );
    void SendUpdateRequest() const;
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    //@}
};

#endif // __Drawing_h_
