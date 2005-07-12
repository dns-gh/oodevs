//*****************************************************************************
//
// Name     : LAU_Workspace.inl
//
// $Created : AML 03-03-24 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 8 $
// $Workfile: LAU_Workspace.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: LAU_Workspace::FindSim
// Object: retourne la SIM avec cet ID ou NULL si aucune
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
INLINE
LAU_SIM* LAU_Workspace::FindSim( const uint32 nExerciceID )
{
    IT_LAU_SIM_PtrVector itSim, itSimEnd;
    for( itSim = listSim_.begin(), itSimEnd = listSim_.end(); itSim != itSimEnd; ++itSim )
    {
         if( (*itSim)->GetExerciceID() == nExerciceID )
         {
            return (*itSim);
         }
    }

    return NULL;
}



//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::GetTimeBetweenTwoPings
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
const MT_TimeSpan& LAU_Workspace::GetTimeBetweenTwoPings() const
{
    return tTimeBetweenTwoPings_;
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::GetTimeMaxWaitPong
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
const MT_TimeSpan& LAU_Workspace::GetTimeMaxWaitPong() const
{
    return tTimeMaxWaitPong_;
}


//-----------------------------------------------------------------------------
// Name: LAU_Workspace::GetTimeMaxWaitInit
// Created: AML 03-06-16
//-----------------------------------------------------------------------------
INLINE
const MT_TimeSpan& LAU_Workspace::GetTimeMaxWaitInit() const
{
    return tTimeMaxWaitInit_;
}
