// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER.h $
// $Author: Jvt $
// $Modtime: 23/03/05 15:57 $
// $Revision: 2 $
// $Workfile: TER.h $
//
// *****************************************************************************

#ifndef __TER_h_
#define __TER_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include "MT_Tools/MT_Vector3D.h"
#include "pathfind/TerrainData.h"
#include <boost/shared_ptr.hpp>

namespace boost
{
    namespace archive
    {
        class binary_oarchive;
        class binary_iarchive;
    };
};

#include <boost/serialization/split_member.hpp>

class TER_Localisation;
typedef std::vector< boost::shared_ptr< TER_Localisation > > T_LocalisationPtrVector;
typedef T_LocalisationPtrVector::iterator                   IT_LocalisationPtrVector;
typedef T_LocalisationPtrVector::const_iterator            CIT_LocalisationPtrVector;

#endif // __TER_h_
