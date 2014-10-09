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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/Protocol.h"

using namespace refugee;
using namespace surrender;

BOOST_CLASS_EXPORT_IMPLEMENT( LodgingAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyRecon< LodgingAttribute > )

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( xml::xistream& xis )
    : capacity_( xis.attribute< unsigned int >( "capacity" ) )
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( const sword::MissionParameter_Value& attributes  )
    : capacity_( attributes.list( 1 ).quantity() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::LodgingAttribute
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute()
    : capacity_( 0 )
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
void LodgingAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> capacity_;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::save
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << capacity_;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Instanciate
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< LodgingAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyRecon< LodgingAttribute >() );
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
bool LodgingAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
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
    return Update( rhs.capacity_ );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Update
// Created: MMC 2012-07-04
// -----------------------------------------------------------------------------
bool LodgingAttribute::Update( unsigned int capacity )
{
    if( capacity_ != capacity )
    {
        capacity_ = capacity;
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::ManageResident
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void LodgingAttribute::ManageResident( MIL_AgentPion& pion, MIL_AgentPion& transporter )
{
    if( std::find( Residents_.begin(), Residents_.end(), &pion ) != Residents_.end() )
        return;

    unsigned int nbrResidents = GetNbrManagedHumansResidents();
    Residents_.push_back( &pion );

    if( capacity_ <= nbrResidents )
    {
        MIL_Report::PostEvent( pion, report::eRC_PrisonersCampFull );
        MIL_Report::PostEvent( transporter, report::eRC_PrisonersCampFull );
        return;
    }

    PHY_RolePion_Refugee* pRefugeeRole = pion.RetrieveRole< PHY_RolePion_Refugee >();
    PHY_RolePion_Surrender* pSurrenderRole = pion.RetrieveRole< PHY_RolePion_Surrender >();

    if( !pRefugeeRole && !pSurrenderRole )
        return;

    unsigned nbrFreeLodging = capacity_ - nbrResidents;
    PHY_RolePion_Composantes& composantes = pion.GetRole< PHY_RolePion_Composantes >();
    unsigned int nbrHumans = composantes.GetNbrLivingHumans();

    if( nbrHumans <= nbrFreeLodging  )
    {
        if( pRefugeeRole )
            pRefugeeRole->UpdateLodging( nbrHumans );
        if( pSurrenderRole )
            pSurrenderRole->UpdateLodging( nbrHumans );
    }
    else
    {
        if( pRefugeeRole )
            pRefugeeRole->UpdateLodging( nbrFreeLodging );
        if( pSurrenderRole )
            pSurrenderRole->UpdateLodging( nbrFreeLodging );
        MIL_Report::PostEvent( pion, report::eRC_PrisonersCampFull );
        MIL_Report::PostEvent( transporter, report::eRC_PrisonersCampFull );
    }
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::UnmanageResident
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void LodgingAttribute::UnmanageResident( MIL_AgentPion& pion )
{
    IT_ResidentsVector it = std::find( Residents_.begin(), Residents_.end(), &pion );
    if( it == Residents_.end() )
        return;

    PHY_RolePion_Refugee* pRefugeeRole = pion.RetrieveRole< PHY_RolePion_Refugee >();
    PHY_RolePion_Surrender* pSurrenderRole = pion.RetrieveRole< PHY_RolePion_Surrender >();

    if( !pRefugeeRole && !pSurrenderRole )
        return;

    unsigned nbrFreeLodging = pRefugeeRole ? pRefugeeRole->GetNbrHumansCampManaged() : pSurrenderRole->GetNbrHumansCampManaged();
    if( pRefugeeRole )
        pRefugeeRole->UpdateLodging( 0 );
    if( pSurrenderRole )
        pSurrenderRole->UpdateLodging( 0 );

    Residents_.erase( it );

    for( CIT_ResidentsVector itResident = Residents_.begin(); itResident != Residents_.end(); ++itResident )
    {
        if( nbrFreeLodging == 0 )
            break;

        MIL_AgentPion* pPion = *itResident;
        PHY_RolePion_Refugee* pCurRefugeeRole = pPion->RetrieveRole< PHY_RolePion_Refugee >();
        PHY_RolePion_Surrender* pCurSurrenderRole = pPion->RetrieveRole< PHY_RolePion_Surrender >();

        if( !pCurRefugeeRole && !pCurSurrenderRole )
            continue;

        unsigned int unmanaged = pCurRefugeeRole ? pCurRefugeeRole->GetNbrHumansCampUnmanaged() : pCurSurrenderRole->GetNbrHumansCampUnmanaged();
        if( unmanaged >= nbrFreeLodging )
        {
            if( pCurRefugeeRole )
                pCurRefugeeRole->UpdateLodging( nbrFreeLodging );
            if( pCurSurrenderRole )
                pCurSurrenderRole->UpdateLodging( nbrFreeLodging );
            break;
        }
        else
        {
            nbrFreeLodging -= unmanaged;
            if( pCurRefugeeRole )
                pCurRefugeeRole->UpdateLodging( unmanaged );
            if( pCurSurrenderRole )
                pSurrenderRole->UpdateLodging( unmanaged );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::GetNbrManagedHumansResidents
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int LodgingAttribute::GetNbrManagedHumansResidents() const
{
    unsigned int count = 0;
    for( CIT_ResidentsVector itResident = Residents_.begin(); itResident != Residents_.end(); ++itResident )
    {
        MIL_AgentPion* pPion = *itResident;
        PHY_RolePion_Refugee* pRefugeeRole = pPion->RetrieveRole< PHY_RolePion_Refugee >();
        PHY_RolePion_Surrender* pSurrenderRole = pPion->RetrieveRole< PHY_RolePion_Surrender >();

        if( !pRefugeeRole && !pSurrenderRole )
            continue;

        count += pRefugeeRole ? pRefugeeRole->GetNbrHumansCampManaged() : pSurrenderRole->GetNbrHumansCampManaged();
    }
    return count;
}
