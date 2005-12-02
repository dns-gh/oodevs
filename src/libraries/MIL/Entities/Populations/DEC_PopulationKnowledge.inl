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
void DEC_PopulationKnowledge::NotifyAttackedBy( const MIL_Agent_ABC& attacker )
{
    newAttackers_.insert( & attacker );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifySecuredBy
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
inline
void DEC_PopulationKnowledge::NotifySecuredBy( const MIL_Agent_ABC& securer )
{
    newSecurers_.insert( & securer );
}
