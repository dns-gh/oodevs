// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ControlZoneCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/MIL_Army.h"
#include "MIL_Random.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ControlZoneCapacity )

namespace
{
    class StaticLoader
    {
    public:
        typedef std::vector< double > T_FirePercentageVector;
    public:
        StaticLoader( T_FirePercentageVector& vFirePercentages )
            : vFirePercentages_ ( vFirePercentages )
        {
            vFirePercentages_.clear();
            vFirePercentages_.resize( PHY_Volume::GetVolumes().size(), 0. );
        }

        //! @name Helpers
        //@{
        void LoadFirePercentages()
        {
            static const char *volumes = "<shot-percentages>"
                "<per-human-per-hectare percentage='100' volume='Medium'/>"
                "<per-human-per-hectare percentage='10' volume='Small'/>"
                "<per-human-per-hectare percentage='20' volume='Personal'/>"
                "<per-human-per-hectare percentage='100' volume='Small'/>"
                "<per-human-per-hectare percentage='10' volume='Medium'/>"
                "<per-human-per-hectare percentage='20' volume='Heavy'/>"
                "<per-human-per-hectare percentage='100' volume='Medium'/>"
                "<per-human-per-hectare percentage='10' volume='Small'/>"
                "<per-human-per-hectare percentage='20' volume='Personal'/>"
                "</shot-percentages>";

            xml::xistringstream xis( volumes );
            xis >> xml::start( "shot-percentages" )
                    >> xml::list( "per-human-per-hectare", *this, &StaticLoader::ReadPercentage )
                >> xml::end;
        }

    private:
        void ReadPercentage( xml::xistream& xis )
        {
            std::string volume;
            double rFirePercentage;

            xis >> xml::attribute( "volume", volume );
            const PHY_Volume* pVolume = PHY_Volume::FindVolume( volume );
            if( !pVolume )
                throw MASA_EXCEPTION( xis.context() + "Unknown volume name - " + volume );

            xis >> xml::attribute( "percentage", rFirePercentage );
            if( rFirePercentage < 0 || rFirePercentage > 100 )
                throw MASA_EXCEPTION( xis.context() + "percentage not in [0..100]" );

            rFirePercentage *= 10000.;                                               // hectare => m2
            rFirePercentage = MIL_Tools::ConvertMeterSquareToSim( rFirePercentage ); // m2 => px2

            vFirePercentages_[ pVolume->GetID() ] = rFirePercentage / 100.;
        }
        //@}
    private:
        T_FirePercentageVector vFirePercentages_;
    };
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity constructor
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
ControlZoneCapacity::ControlZoneCapacity( const MIL_Agent_ABC& controller )
    : controller_ ( &controller )
    , vFirerPosition_ ( &controller.GetRole< PHY_RoleInterface_Location >().GetPosition() )
{
    StaticLoader loader( vFirePercentages_ );
    loader.LoadFirePercentages();
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ControlZoneCapacity::ControlZoneCapacity()
    : controller_ ( 0 )
    , vFirerPosition_ ( 0 )
{
    StaticLoader loader( vFirePercentages_ );
    loader.LoadFirePercentages();
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ControlZoneCapacity::ControlZoneCapacity( const ControlZoneCapacity& from )
    : controller_ ( from.controller_ )
    , vFirerPosition_ ( from.vFirerPosition_ )
    , vFirePercentages_ ( from.vFirePercentages_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacitydestructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ControlZoneCapacity::~ControlZoneCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void ControlZoneCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & const_cast< MIL_Agent_ABC*& >( controller_ );
    if( controller_ )
        vFirerPosition_ = &controller_->GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ControlZoneCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ControlZoneCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new ControlZoneCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::GetUnitDensityFirePercentage
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
double ControlZoneCapacity::GetUnitDensityFirePercentage( const PHY_Volume& volume ) const
{
    return vFirePercentages_[ volume.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::RetrieveTargets
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
void ControlZoneCapacity::RetrieveTargets( const MIL_Object_ABC& object, T_TargetVector& targets ) const
{
    double area = object.GetLocalisation().GetArea();
    double rPHCoeff = MT_IsZero( area )
                                ? 0
                                : controller_->GetRole< human::PHY_RoleInterface_Humans >().GetNbrUsableHumans() / area;
    targets.clear();
    object.ProcessAgentsInside( boost::bind( &ControlZoneCapacity::ControlTarget, this, _1, object.GetArmy(), rPHCoeff, boost::ref( targets ) ) );
}

// -----------------------------------------------------------------------------
// Name: ControlZoneCapacity::ControlTarget
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
void ControlZoneCapacity::ControlTarget( MIL_Agent_ABC* agent, const MIL_Army_ABC* army, double phCoef, T_TargetVector& targets ) const
{
    if( army && army->IsAFriend( agent->GetArmy() ) == eTristate_True )
        return;
    PHY_Composante_ABC::T_ComposanteVector compTargets;
    agent->GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( compTargets );
    for( auto itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
    {
        PHY_Composante_ABC& compTarget = **itCompTarget;
        if( MIL_Random::rand_oi( 0., 1. ) <= phCoef * GetUnitDensityFirePercentage( compTarget.GetType().GetVolume() ) )
            targets.push_back( std::make_pair( agent, &compTarget ) );
    }
}
