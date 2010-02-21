// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingPositions_h_
#define __DrawingPositions_h_

#include "clients_gui/DrawingPositions.h"
#include "protocol/messengersenders.h"

using namespace Common;


namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  DrawingPositions
    @brief  DrawingPositions
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingPositions : public gui::DrawingPositions
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawingPositions();
             DrawingPositions( const kernel::CoordinateConverter_ABC& converter, const MsgsMessengerToClient::MsgShapeCreation& message );
    virtual ~DrawingPositions();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingPositions( const DrawingPositions& );            //!< Copy constructor
    DrawingPositions& operator=( const DrawingPositions& ); //!< Assignment operator
    //@}
};

#endif // __DrawingPositions_h_
