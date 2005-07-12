//*****************************************************************************
//
// $Created: AML 03-04-17 $
// $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP.h $
// $Author: Aml $
// $Modtime: 17/04/03 15:05 $
// $Revision: 2 $
// $Workfile: LMP.h $
//
//*****************************************************************************

#ifndef __LMP_h_
#define __LMP_h_

#ifdef  _DEBUG
#   undef USE_INLINE
#   undef   INLINE                  
#   define  INLINE 
#else  
#   define  USE_INLINE
#   undef   INLINE                  
#   define  INLINE inline
#endif

#endif // __LMP_h_