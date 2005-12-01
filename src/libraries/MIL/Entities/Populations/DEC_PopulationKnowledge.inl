// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationKnowledge.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 14:34 $
// $Revision: 5 $
// $Workfile: DEC_PopulationKnowledge.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
inline
void DEC_PopulationKnowledge::NotifyAttackedBy( MIL_Agent_ABC& attacker )
{
    newAttackers_.insert( & attacker );
}
