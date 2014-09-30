// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef DefaultVisualisationScales_h
#define DefaultVisualisationScales_h

#include "ENT/ENT_Enums.h"

namespace kernel
{
    struct DefaultVisualisationScales
    {
        E_VisualisationScale                    type_;
        int                                     min_;
        int                                     max_;
        static const DefaultVisualisationScales data_[];
        static const size_t                     size_;
    };
}

#endif // DefaultVisualisationScales_h
