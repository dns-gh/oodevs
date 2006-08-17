// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_RealObjectTypeFilter.h $
// $Author: Nld $
// $Modtime: 26/10/04 16:00 $
// $Revision: 1 $
// $Workfile: MIL_RealObjectTypeFilter.h $
//
// *****************************************************************************

#ifndef __MIL_RealObjectTypeFilter_h_
#define __MIL_RealObjectTypeFilter_h_

#include "MIL.h"

class MIL_RealObjectType;

// =============================================================================
// @class  MIL_RealObjectTypeFilter
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RealObjectTypeFilter
{
    MT_COPYNOTALLOWED( MIL_RealObjectTypeFilter )

public:
     MIL_RealObjectTypeFilter();
     MIL_RealObjectTypeFilter( DIA_Parameters& diaParameters, uint nFirstDiaParam );     
    ~MIL_RealObjectTypeFilter();

    //! @name 
    //@{
    void Set  ();
    void Set  ( const MIL_RealObjectType& objectType );
    void Reset( const MIL_RealObjectType& objectType );
    bool Test ( const MIL_RealObjectType& objectType ) const;
    
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< bool >                     T_ObjectTypeStateVector; 
    typedef const T_ObjectTypeStateVector::iterator CIT_ObjectTypeStateVector;
    //@}

private:
    T_ObjectTypeStateVector objectTypeStates_;
};

#endif // __MIL_RealObjectTypeFilter_h_
