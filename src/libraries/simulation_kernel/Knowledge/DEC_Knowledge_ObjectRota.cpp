// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectRota.cpp $
// $Author: Nld $
// $Modtime: 26/04/05 14:17 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_ObjectRota.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectRota.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_Rota.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectRota, "DEC_Knowledge_ObjectRota" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectRota::DEC_Knowledge_ObjectRota( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , bSpecificAttributesValid_( false )
    , nDanger_                 ( 0 )
{
    asnAttributes_.agents_nbc.n = 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectRota::DEC_Knowledge_ObjectRota()
    : DEC_Knowledge_Object     ()
    , bSpecificAttributesValid_( false )
    , nDanger_                 ( 0 )
{
    asnAttributes_.agents_nbc.n = 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectRota::~DEC_Knowledge_ObjectRota()
{
    CleanMsgSpecificAttributes();   
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive > 
        inline
        void serialize( Archive& file, DEC_Knowledge_ObjectRota::T_NbcAgentSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_ObjectRota::T_NbcAgentSet& set, const uint )
        {
            unsigned size = set.size();
            file << size;
            for ( DEC_Knowledge_ObjectRota::CIT_NbcAgentSet it = set.begin(); it != set.end(); ++it )
            {
                unsigned id = (*it)->GetID();
                file << id;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_ObjectRota::T_NbcAgentSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                file >> nID;
                set.insert( MIL_NbcAgentType::Find( nID ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_ObjectRota::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    archive & bSpecificAttributesValid_
            & nbcAgents_
            & nDanger_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_Rota& rota = static_cast< const MIL_Rota& >( *GetObjectKnown() );
        nDanger_   = rota.GetDanger();        
        nbcAgents_ = rota.GetNbcAgents();

        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception )
{
    if( !IsReconBy( perception.GetAgentPerceiving().GetType() ) )
        return;
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& collision )
{
    if( !IsReconBy( collision.GetAgentColliding().GetType() ) )
        return;
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    CleanMsgSpecificAttributes();

    asnAttributes_.niveau_danger = nDanger_;
    asnAttributes_.agents_nbc.n = nbcAgents_.size();
    if( !nbcAgents_.empty() )
    {
        asnAttributes_.agents_nbc.elem = new ASN1T_OID[ nbcAgents_.size() ];
        uint i = 0;
        for( CIT_NbcAgentSet itNbc = nbcAgents_.begin(); itNbc != nbcAgents_.end(); ++itNbc, ++i )
            asnAttributes_.agents_nbc.elem[ i ]  = (**itNbc).GetID();
    }

    asnMsg.m.attributs_specifiquesPresent = 1;
    asnMsg.attributs_specifiques.t        = T_ObjectAttributesSpecific_rota;
    asnMsg.attributs_specifiques.u.rota   = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectRota::CleanMsgSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectRota::CleanMsgSpecificAttributes()
{
    if( asnAttributes_.agents_nbc.n > 0 )
        delete [] asnAttributes_.agents_nbc.elem;
}
