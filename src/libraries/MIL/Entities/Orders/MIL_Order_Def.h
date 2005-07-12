//*****************************************************************************
//
// $Created: NLD 2003-01-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Order_Def.h $
// $Author: Nld $
// $Modtime: 3/09/04 11:47 $
// $Revision: 1 $
// $Workfile: MIL_Order_Def.h $
//
//*****************************************************************************

#ifndef __MIL_Order_Def_h_
#define __MIL_Order_Def_h_

// Useful types
class MIL_Lima;

typedef std::map< const MIL_Lima*, bool/*bFlaged*/ > T_LimaFlagedPtrMap;
typedef T_LimaFlagedPtrMap::iterator                 IT_LimaFlagedPtrMap;
typedef T_LimaFlagedPtrMap::const_iterator           CIT_LimaFlagedPtrMap;

typedef std::vector< const MIL_Lima* >       T_LimaConstPtrVector;
typedef const T_LimaConstPtrVector           CT_LimaConstPtrVector;
typedef T_LimaConstPtrVector::iterator       IT_LimaConstPtrVector;
typedef T_LimaConstPtrVector::const_iterator CIT_LimaConstPtrVector;

#endif // __MIL_Order_Def_h_
