// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LightingHelpers_h_
#define __LightingHelpers_h_

namespace gui
{
    namespace lighting
    {
        enum E_Type
        {
            eFixed,
            eCameraFixed,
            eSimulationTime
        };

        QRect GetEditorRect();
        geometry::Vector3f PointToDirectionVector( QPoint& position );
    } //! namespace lighting
} //! namespace gui

#endif // __LightingHelpers_h_
