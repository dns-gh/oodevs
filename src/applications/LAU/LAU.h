//*****************************************************************************
//
// $Created: AML 03-04-16 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU.h $
// $Author: Aml $
// $Modtime: 15/05/03 15:45 $
// $Revision: 6 $
// $Workfile: LAU.h $
//
//*****************************************************************************

#ifndef __LAU_h_
#define __LAU_h_

#define LAU_DEFAULT_NUM_PORT_MOS        4321
#define LAU_DEFAULT_NUM_PORT_SIM        4322
#define LAU_DEFAULT_MAGIC_CONNECTION    3232

enum LAU_Connector
{
//    eConnector_SIM_MOS = (DIN::DIN_Connector_ABC::DIN_ConnectionID)-1,    // pas utile de le connaitre ici
    eConnector_SIM_LAU = 1,
    eConnector_MOS_LAU = 2
};

#define LAU_IS_A_SERVICE    // a definir pour que le launcher soit un service

#ifdef  _DEBUG
#   undef USE_INLINE
#   undef   INLINE                  
#   define  INLINE 
#else  
#   define  USE_INLINE
#   undef   INLINE                  
#   define  INLINE inline
#endif


#endif // __LAU_h_