// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingPositions_h_
#define __DrawingPositions_h_

#include "clients_gui/DrawingPositions.h"
#include "protocol/MessengerSenders.h"

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
             DrawingPositions( const kernel::CoordinateConverter_ABC& converter, const sword::ShapeCreation& message );
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
