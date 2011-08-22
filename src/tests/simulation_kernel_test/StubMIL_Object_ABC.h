// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StubMIL_Object_ABC_h_
#define __StubMIL_Object_ABC_h_

#include "Entities/Objects/MIL_Object_ABC.h"

// =============================================================================
/** @class  StubMIL_Object_ABC
    @brief  StubMIL_Object_ABC
*/
// Created: LDC 2010-01-04
// =============================================================================
class StubMIL_Object_ABC : public MIL_Object_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StubMIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int id )
                 : MIL_Object_ABC( army, type, id )
                 , name_( "" )
             {
                 // NOTHING
             }
    virtual ~StubMIL_Object_ABC()
             {
                 // NOTHING
             }
    //@}

    //! @name Operations
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const {}
    virtual void PreprocessAgent( MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentEntering( MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentExiting( MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessAgentInside( MIL_Agent_ABC& /*agent*/ ) {}
    virtual void ProcessPopulationInside( MIL_PopulationElement_ABC& /*population*/ ) {}
    virtual void PreprocessPopulation( MIL_PopulationElement_ABC& /*population*/ ) {};
    virtual void ApplyIndirectFire( const TER_Localisation& /*attritionSurface*/, const PHY_DotationCategory& /*dotation*/ ) {};
    virtual void ApplyDirectFire() const {};
    virtual void ApplyDestruction( const TER_Localisation& /*attritionSurface*/, const PHY_UrbanAttritionData& /*attrition*/ ) {};
    virtual bool IsUniversal() const { throw; };
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& /*team*/ ) { throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& /*group*/ ) { throw; }
    virtual const MIL_ObjectManipulator_ABC& operator()() const { throw; }
    virtual MIL_ObjectManipulator_ABC& operator()() { throw; }
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const sword::MissionParameter_Value& /*asn*/ ) { throw; }
    virtual void SendCreation() const {}
    virtual void SendDestruction() const {}
    virtual void SendFullState() const {}
    virtual void Update( unsigned int /*time*/ ) {}
    virtual void Register( ObjectCapacity_ABC* ) { throw; }
    virtual void Register( ObjectAttribute_ABC* ) { throw; }
    virtual void Register( MIL_StructuralStateNotifier_ABC& ) {}
    virtual void Register( MIL_InteractiveContainer_ABC* ) {}
    virtual void ApplyStructuralState( float /*structuralState*/ ) const {}
    virtual void Instanciate( MIL_Object_ABC& /*object*/ ) const {}
    virtual void Finalize() {}
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& /*attributes*/ ) { throw; }
    virtual const std::string& GetName() const { return name_; };
    //@}

private:
    //! @name Member Data
    //@{
    const std::string name_;
    //@}
};

#endif // __StubMIL_Object_ABC_h_
