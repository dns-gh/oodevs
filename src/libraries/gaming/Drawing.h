// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
             Drawing( kernel::Controllers& controllers, const sword::ShapeCreation& asn, const kernel::Entity_ABC* entity, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Drawing( kernel::Controllers& controllers, const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Drawing( kernel::Controllers& controllers, xml::xistream& xis, const kernel::Entity_ABC* entity, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ShapeUpdate& message );
    virtual void NotifyDestruction() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Create();
    virtual void Update();
    //@}

    //! @name Helpers
    //@{
    void SetLocation( const sword::CoordLatLongList& list );
    void SerializeLocation( sword::CoordLatLongList& list ) const;
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    bool publishUpdate_;
    //@}
};

#endif // __Drawing_h_
