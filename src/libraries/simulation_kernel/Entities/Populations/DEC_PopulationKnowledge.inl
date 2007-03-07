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

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::GetChannelingLocations
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
inline
const DEC_PopulationKnowledge::T_LocationVector& DEC_PopulationKnowledge::GetChannelingLocations() const
{
    return channelingLocations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::HasChannelingChanged
// Created: SBO 2006-01-17
// -----------------------------------------------------------------------------
inline
bool DEC_PopulationKnowledge::HasChannelingChanged() const
{
    return bChannelingChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationKnowledge::NotifyChanneling
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
inline
void DEC_PopulationKnowledge::NotifyChanneled( const TER_Localisation& location )
{
    CIT_LocationVector it = std::find( channelingLocations_.begin(), channelingLocations_.end(), location );
    if( it == channelingLocations_.end() )
        bNewChannelingChanged_ = true;
    newChannelingLocations_.push_back( location );
}
