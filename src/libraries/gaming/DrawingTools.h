// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GAMING_DRAWING_TOOLS_H
#define GAMING_DRAWING_TOOLS_H

namespace plugins
{
    namespace messenger
    {
        class ShapeCreationRequest;
        class ShapeUpdateRequest;
    }
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace tools
{
    void SerializeLocation( plugins::messenger::ShapeCreationRequest& message,
                            const kernel::CoordinateConverter_ABC& converter,
                            kernel::Location_ABC& location );
    void SerializeLocation( plugins::messenger::ShapeUpdateRequest& message,
                            const kernel::CoordinateConverter_ABC& converter,
                            kernel::Location_ABC& location );
}  //! namespace tools

#endif  // GAMING_DRAWING_TOOLS_H
