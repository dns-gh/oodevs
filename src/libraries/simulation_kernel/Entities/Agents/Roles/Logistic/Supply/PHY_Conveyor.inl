// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_Conveyor.inl $
// $Author: Nld $
// $Modtime: 19/07/05 18:00 $
// $Revision: 1 $
// $Workfile: PHY_Conveyor.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::IsFull
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
inline
bool PHY_Conveyor::IsFull() const
{
    return rWeightCapacity_ <= 0. || rVolumeCapacity_ <= 0.;
}

