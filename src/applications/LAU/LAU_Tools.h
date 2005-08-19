//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 15 $
// $Workfile: LAU_SIM.h $
//
//*****************************************************************************

#ifndef __LAU_Tools_h_
#define __LAU_Tools_h_

// on redefini un system de log a nous car sinon ca marche pas bien appellé depuis un service pquoi ? ben je sais pas!!!!
const char* MT_GetApplicationPath( const char* pFilenameToConcatene = NULL );
void OutputLogDebug( const char* szMsg, const char* szSourceFilename = NULL, const int nNumLine = -1 );
const char* SystemGetErrorMessage( const int nNumSystemError );

#endif // __LAU_SIM_h_
