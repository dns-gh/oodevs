// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher
{
   
// -----------------------------------------------------------------------------
// Name: EquipmentAvailability constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
EquipmentAvailability< T >::EquipmentAvailability( Model& /*model*/, const T& asn )
    : nEquipmentType_( asn.type_equipement  )
    , nNbrTotal_     ( asn.nbr_total        )
    , nNbrAvailable_ ( asn.nbr_disponibles  )
    , nNbrWorking_   ( asn.nbr_au_travail   )
    , nNbrLent_      ( asn.nbr_pretes       )
    , nNbrResting_   ( asn.m.nbr_au_reposPresent ? asn.nbr_au_repos : std::numeric_limits< unsigned int >::max() )
{
}

// -----------------------------------------------------------------------------
// Name: EquipmentAvailability destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
EquipmentAvailability< T >::~EquipmentAvailability()
{
}

// -----------------------------------------------------------------------------
// Name: EquipmentAvailability::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
void EquipmentAvailability< T >::Send( T& asn ) const
{
    asn.type_equipement  = nEquipmentType_;
    asn.nbr_total        = nNbrTotal_;
    asn.nbr_disponibles  = nNbrAvailable_;
    asn.nbr_au_travail   = nNbrWorking_;
    asn.nbr_pretes       = nNbrLent_;
    if( nNbrResting_ != std::numeric_limits< unsigned int >::max() )
    {
        asn.m.nbr_au_reposPresent = 1;
        asn.nbr_au_repos = nNbrResting_;
    }   
}

}