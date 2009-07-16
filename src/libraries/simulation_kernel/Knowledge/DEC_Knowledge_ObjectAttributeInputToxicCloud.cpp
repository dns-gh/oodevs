#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeInputToxicCloud.h"
#include "Entities/Objects/InputToxicCloudAttribute.h"
#include "DEC_Knowledge_Object.h"


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud constructor
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInputToxicCloud::DEC_Knowledge_ObjectAttributeInputToxicCloud() 
    : attr_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud constructor
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInputToxicCloud::DEC_Knowledge_ObjectAttributeInputToxicCloud( const InputToxicCloudAttribute& attr ) 
    : attr_(&attr)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud destructor
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInputToxicCloud::~DEC_Knowledge_ObjectAttributeInputToxicCloud()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::serialize
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeInputToxicCloud::serialize( Archive&, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    //
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateOnPerceptionLevel
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel )
{
    UpdateAttributes();	
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateOnPerception
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::UpdateOnCollision
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::Send
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateAttributes
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInputToxicCloud::UpdateAttributes()
{
    //attr_->Update(time,polygon_);
}
