// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LodgingAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace refugee;

BOOST_CLASS_EXPORT_IMPLEMENT( LodgingAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyRecon< LodgingAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyRecon< LodgingAttribute > )

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( xml::xistream& xis )
{
    xis >> xml::attribute( "lodging-capacity", capacity_ );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( const sword::MissionParameter_Value& attributes  )
    : capacity_ ( 0 )
{
    capacity_ = attributes.list( 1 ).quantity();
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::LodgingAttribute
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute()
: capacity_ ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: LodgingAttribute destructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::~LodgingAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::WriteODB
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "lodging" )
        << xml::attribute( "capacity", capacity_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::load
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Load( xml::xistream& xis )
{
    capacity_ = xis.attribute< unsigned int >( "capacity", 0 );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::load
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       >> capacity_;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::save
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       << capacity_;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Instanciate
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< LodgingAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Register
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< LodgingAttribute, LodgingAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void LodgingAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_lodging()->set_capacity( capacity_ );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Send
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: DotationAttribute::operator=
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute& LodgingAttribute::operator=( const LodgingAttribute& rhs )
{
    capacity_ = rhs.capacity_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Update
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
bool LodgingAttribute::Update( const LodgingAttribute& rhs )
{
    if( capacity_ != rhs.capacity_ )
    {
        capacity_ = rhs.capacity_;
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::ManageRefugee
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void LodgingAttribute::ManageRefugee( MIL_AgentPion& pion )
{
    if ( std::find( refugees_.begin(), refugees_.end(), &pion ) != refugees_.end() )
        return;

    unsigned int nbrRefugees = GetNbrManagedHumansRefugees();
    refugees_.push_back( &pion );

    if ( capacity_ <= nbrRefugees )
        return;

    unsigned nbrFreeLodging = capacity_ - nbrRefugees;
    PHY_RolePion_Composantes& composantes = pion.GetRole< PHY_RolePion_Composantes >();
    PHY_RolePion_Refugee& refugeeRole = pion.GetRole< PHY_RolePion_Refugee >();
    unsigned int nbrHumans = composantes.GetNbrUsableHumans();    

    if ( nbrHumans <= nbrFreeLodging  )
        refugeeRole.UpdateLodgingSatisfaction( nbrHumans );
    else
        refugeeRole.UpdateLodgingSatisfaction( nbrFreeLodging );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::UnmanageRefugee
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void LodgingAttribute::UnmanageRefugee( MIL_AgentPion& pion )
{
    IT_RefugeesVector it = std::find( refugees_.begin(), refugees_.end(), &pion );
    if ( it == refugees_.end() )
        return;
    
    PHY_RolePion_Refugee& refugeeRole = pion.GetRole< PHY_RolePion_Refugee >();
    unsigned nbrFreeLodging = refugeeRole.GetNbrHumansCampManaged();
    refugeeRole.UpdateLodgingSatisfaction( 0 );
    refugees_.erase( it );

    for ( CIT_RefugeesVector itRefugee = refugees_.begin(); itRefugee != refugees_.end(); ++itRefugee )
    {
        if ( nbrFreeLodging == 0 )
            break;

        MIL_AgentPion* pPion = *itRefugee;
        PHY_RolePion_Refugee& curRefugeeRole = pPion->GetRole< PHY_RolePion_Refugee >();
        unsigned int unmanaged = curRefugeeRole.GetNbrHumansCampUnmanaged();
        if ( unmanaged >= nbrFreeLodging )
        {
            curRefugeeRole.UpdateLodgingSatisfaction( nbrFreeLodging );
            break;
        }
        else
        {
            nbrFreeLodging -= unmanaged;
            curRefugeeRole.UpdateLodgingSatisfaction( unmanaged );
        }
    }
}


// -----------------------------------------------------------------------------
// Name: LodgingAttribute::GetNbrManagedHumansRefugees
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int LodgingAttribute::GetNbrManagedHumansRefugees() const
{
    unsigned count = 0;
    for ( CIT_RefugeesVector itRefugee = refugees_.begin(); itRefugee != refugees_.end(); ++itRefugee )
    {
        MIL_AgentPion* pPion = *itRefugee;
        PHY_RolePion_Refugee& refugeeRole = pPion->GetRole< PHY_RolePion_Refugee >();
        count += refugeeRole.GetNbrHumansCampManaged();
    }
    return count;
}