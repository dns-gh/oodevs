//*****************************************************************************
//
// $Created: JVT 04-03-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_GaussianRandom.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:43 $
// $Revision: 2 $
// $Workfile: MT_GaussianRandom.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetMean
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
inline
double MT_GaussianRandom::GetMean() const
{
    return rMean_;
}


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetVariance
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
inline
double MT_GaussianRandom::GetVariance() const
{
    return rVariance_;
}

