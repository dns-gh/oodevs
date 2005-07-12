//*****************************************************************************
//
// Name     : LAU_MOS.h
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 5 $
// $Workfile: LAU_MOS.h $
//
//*****************************************************************************

#ifndef __LAU_MOS_h_
#define __LAU_MOS_h_

#include "LAU.h"
#include "LAU_Client_ABC.h"

//*****************************************************************************
// Object : L'image d'un MOS distant
// Created: AML 03-03-26
//*****************************************************************************
class LAU_MOS : public LAU_Client_ABC
{
    MT_COPYNOTALLOWED( LAU_MOS )

public:
    explicit LAU_MOS();
    virtual ~LAU_MOS();
    
    void Reset( void );
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    //@}

private:
};

// Add Normalized Access Facilities

typedef std::vector< LAU_MOS > T_LAU_MOS_Vector;
typedef T_LAU_MOS_Vector::iterator IT_LAU_MOS_Vector;
typedef T_LAU_MOS_Vector::const_iterator CIT_LAU_MOS_Vector;
typedef T_LAU_MOS_Vector::reverse_iterator RIT_LAU_MOS_Vector;
typedef T_LAU_MOS_Vector::const_reverse_iterator CRIT_LAU_MOS_Vector;

typedef std::vector< LAU_MOS* > T_LAU_MOS_PtrVector;
typedef T_LAU_MOS_PtrVector::iterator IT_LAU_MOS_PtrVector;
typedef T_LAU_MOS_PtrVector::const_iterator CIT_LAU_MOS_PtrVector;
typedef T_LAU_MOS_PtrVector::reverse_iterator RIT_LAU_MOS_PtrVector;
typedef T_LAU_MOS_PtrVector::const_reverse_iterator CRIT_LAU_MOS_PtrVector;

#ifdef USE_INLINE
#   include "LAU_MOS.inl"
#endif 


#endif // __LAU_MOS_h_
