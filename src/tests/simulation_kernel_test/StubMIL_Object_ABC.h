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
             StubMIL_Object_ABC() {}
    virtual ~StubMIL_Object_ABC() {}
    //@}

    virtual void WriteODB( xml::xostream& xos ) const {}
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent ) {}
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent ) {}
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent ) {}
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent ) {}
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population ) {}
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team ) { throw; }
    virtual const MIL_ObjectManipulator_ABC& operator()() const { throw; }
    virtual       MIL_ObjectManipulator_ABC& operator()() { throw; }
    virtual HLA_Object_ABC* GetHLAView() const { throw; }
    virtual void            SetHLAView( HLA_Object_ABC& view ) {}
    virtual void    Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer ) {}
    virtual void    Serialize  ( HLA_UpdateFunctor& functor ) const {}
    virtual ASN1T_EnumObjectErrorCode OnUpdate( const ASN1T_ObjectAttributes& asn ) { throw; }
    virtual void    SendCreation() const {}
    virtual void    SendDestruction() const {}
    virtual void    SendFullState() const {}
    virtual uint              GetID() const { throw; }
    virtual void Update( uint time ) {}
};

#endif // __StubMIL_Object_ABC_h_
