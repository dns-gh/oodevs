// *****************************************************************************
//
// $Created: JVT 2005-03-23 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/CheckPoints/MIL_CheckPointSerializationHelpers.h $
// $Author: Jvt $
// $Modtime: 31/03/05 12:10 $
// $Revision: 1 $
// $Workfile: MIL_CheckPointSerializationHelpers.h $
//
// *****************************************************************************

#ifndef __MIL_CheckPointSerializationHelpers_h_
#define __MIL_CheckPointSerializationHelpers_h_

#include <boost/serialization/split_free.hpp>

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, MT_Vector2D& vect, const unsigned int )
        {
            file & vect.rX_;
            file & vect.rY_;
        }
    }
}

#endif // __MIL_CheckPointSerializationHelpers_h_
