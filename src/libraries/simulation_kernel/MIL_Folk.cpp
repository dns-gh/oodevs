// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Folk.h"
#include "Tools/MIL_Config.h"

#include "population/Config.h"
#include "population/FlowManagerFactory.h"
#include "population/FlowManager_ABC.h"
#include "population/Serializer_ABC.h"
#include "population/Time.h"
#include "Network/NET_ASN_Messages.h"

using namespace population;

// -----------------------------------------------------------------------------
// Name: MIL_Folk constructor
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
MIL_Folk::MIL_Folk( const MIL_Config& config )
    : pFlow_ ( ( config.IsPopulationEnabled() ) ? population::FlowManagerFactory::Create() : 0 )
    , first_update_ ( true )
{    
    if ( pFlow_ )
    {
        MT_LOG_INFO_MSG( "Loading population module" )
        Config  popConfig( config.GetPopulationDir(), config.GetPopulationFile() );
        pFlow_->Load( popConfig );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk destructor
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
MIL_Folk::~MIL_Folk()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::Update
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::Update( unsigned utime, unsigned timeStep )
{
    if ( pFlow_ )
    {
        pFlow_->Update( population::Time( "", utime ), timeStep );
        UpdateNetwork();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendStateToNewClient
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
void MIL_Folk::SendStateToNewClient() const
{
    if ( pFlow_ )
    {
        SendCreation();
        SendUpdate();
        first_update_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::UpdateNetwork
// Created: JCR 2007-08-27
// -----------------------------------------------------------------------------
void MIL_Folk::UpdateNetwork() const
{
    if ( !first_update_ )
        SendUpdate();
}

namespace 
{
    template< typename T >
    void CopyList( T& list, const std::vector<const std::string*>& value )
    {
        list.n = value.size();
        list.elem = new ASN1TDynOctStr[list.n];
        for( unsigned int i = 0; i < list.n; ++i )
            list.elem[i] = value[i]->c_str();
    }

    struct FolkCreationSerializer : public population::message::Serializer_ABC
    {
        explicit FolkCreationSerializer( NET_ASN_MsgFolkCreation& asn ) : asn_ ( &asn ) 
        {
            (*asn_)().profiles.n = 0;
            (*asn_)().activities.n = 0;
        }

        virtual population::message::Serializer_ABC& operator <<( const std::vector<const std::string*>& value )
        {
            if ( (*asn_)().profiles.n == 0 )
                CopyList( (*asn_)().profiles, value );
            else
                CopyList( (*asn_)().activities, value );
            return *this;
        }
        NET_ASN_MsgFolkCreation* asn_;
    };

    struct FolkUpdateSerializer : public population::message::Serializer_ABC
    {
        FolkUpdateSerializer() { Clear(); }

        void Clear()
        {
            asn_().oid = 0;
            asn_().population_occupation.n = 0;
            asn_().population_occupation.elem = 0;
        }

        // $$$$ JCR 2007-08-27:  not needed yet...
        virtual population::message::Serializer_ABC& operator <<( int value )
        {
            asn_().oid = value;            
            return *this;
        }

        virtual population::message::Serializer_ABC& operator <<( const std::vector<int>& value )
        {
            asn_().population_occupation.n = value.size();
            asn_().population_occupation.elem = &*const_cast<std::vector<int>&>(value).begin();
            asn_.Send();
            Clear();
            return *this;
        } 
        
        NET_ASN_MsgFolkGraphEdgeUpdate asn_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendCreation
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::SendCreation() const
{    
    NET_ASN_MsgFolkCreation asn;
    FolkCreationSerializer  serializer( asn );
    
    MT_LOG_INFO_MSG( "MIL_Folk::SendCreation()" )
    pFlow_->SerializePopulationCreation( serializer );
    asn.Send();
    delete [] asn().profiles.elem;
    delete [] asn().activities.elem;    
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendUpdate
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::SendUpdate() const
{
    FolkUpdateSerializer    serializer;    
    pFlow_->SerializePopulationUpdate( serializer );
}
