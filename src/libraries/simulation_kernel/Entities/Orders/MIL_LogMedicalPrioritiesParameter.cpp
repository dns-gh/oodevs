// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LogMedicalPrioritiesParameter.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LogMedicalPrioritiesParameter )

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_LogMedicalPrioritiesParameter::MIL_LogMedicalPrioritiesParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMedicalPrioritiesParameter::MIL_LogMedicalPrioritiesParameter( const sword::LogMedicalPriorities & asn )
{
    priorities_.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        const PHY_HumanWound* pWound = PHY_HumanWound::Find( asn.elem( i ) );
        if( !pWound )
        {
            priorities_.clear();
            break;
        }
        priorities_.push_back( pWound );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMedicalPrioritiesParameter::~MIL_LogMedicalPrioritiesParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eMedicalPriorities;
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities( T_MedicalPriorityVector& value ) const
{
    value = priorities_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    for( std::size_t i = 0; i < priorities_.size(); ++i )
        elem.mutable_logmedicalpriorities()->add_elem( priorities_[ i ]->GetAsnID() );
    return true;
}

namespace boost
{
    namespace serialization
    {
        typedef std::vector< const PHY_HumanWound* >      T_MedicalPriorityVector;
        typedef T_MedicalPriorityVector::const_iterator CIT_MedicalPriorityVector;

        // =============================================================================
        // T_MedicalPriorityVector
        // =============================================================================
        template< typename Archive >
        inline
            void serialize( Archive& file, T_MedicalPriorityVector& vector, const unsigned int nVersion )
        {
            split_free( file, vector, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const T_MedicalPriorityVector& vector, const unsigned int )
        {
            std::size_t size = vector.size();
            for( auto it = vector.begin(); it != vector.end(); ++it )
            {
                if( !*it )
                    --size;
            }
            file << size;
            for( auto it = vector.begin(); it != vector.end(); ++it )
            {
                if( !*it )
                    continue;
                unsigned id = (*it)->GetID();
                file << id;
            }
        }

        template< typename Archive >
        void load( Archive& file, T_MedicalPriorityVector& vector, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while( nNbr-- )
            {
                unsigned int nID;
                file >> nID;
                vector.push_back( PHY_HumanWound::Find( nID ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::load
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_LogMedicalPrioritiesParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this )
         >> priorities_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::save
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_LogMedicalPrioritiesParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this )
         << priorities_;
}
