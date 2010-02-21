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
#include "protocol/messengersenders.h"
#include "protocol/publisher_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class DrawingTypes;
}
namespace Common
{
    class MsgCoordLatLongList;
}

class Publisher_ABC;

// =============================================================================
/** @class  Drawing
    @brief  Drawing
*/
// Created: SBO 2008-06-04
// =============================================================================
class Drawing : public gui::DrawerShape
              , public kernel::Updatable_ABC< MsgsMessengerToClient::MsgShapeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             Drawing( kernel::Controller& controller, const MsgsMessengerToClient::MsgShapeCreation& asn, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Drawing( kernel::Controller& controller, const gui::DrawingTemplate& style, const QColor& color, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Drawing( kernel::Controller& controller, xml::xistream& xis, const gui::DrawingTypes& types, kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsMessengerToClient::MsgShapeUpdate& message );
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Drawing( const Drawing& );            //!< Copy constructor
    Drawing& operator=( const Drawing& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Create();
    virtual void Update();
    //@}

    //! @name Helpers
    //@{
    void SetLocation( const Common::MsgCoordLatLongList& list );
    void SerializeLocation( Common::MsgCoordLatLongList& list ) const;
    void CleanLocation( Common::MsgCoordLatLongList& list ) const;
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
