// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObjectWrapper_h_
#define __UrbanObjectWrapper_h_

#include "MIL_Object.h"
#include <map>

class MIL_ObjectBuilder_ABC;

namespace sword
{
    enum UrbanMagicActionAck_ErrorCode;
    class UrbanAttributes;
    class UrbanMagicAction_Infrastructure;
}

namespace urban
{
    class TerrainObject_ABC;
    class MotivationsVisitor_ABC;
}

// =============================================================================
/** @class  UrbanObjectWrapper
    @brief  Urban object wrapper
*/
// Created: SLG 2010-06-18
// =============================================================================
class UrbanObjectWrapper : public MIL_Object
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object );
             UrbanObjectWrapper();
    virtual ~UrbanObjectWrapper();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    void WriteUrbanIdAttribute( xml::xostream& xos ) const;
    //@}

    //! @name Interaction
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team );
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group );
    //@}

    //! @name Network
    //@{
    sword::UrbanMagicActionAck_ErrorCode OnUpdateStructuralState( int state );
    sword::UrbanMagicActionAck_ErrorCode OnUpdateInfrastructure( const sword::UrbanMagicAction_Infrastructure& msg );

    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );
    virtual sword::ObjectMagicActionAck_ErrorCode OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );

    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    virtual void UpdateState();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetUrbanId() const;
    const std::string& GetName() const;
    const urban::TerrainObject_ABC& GetObject();
    const urban::TerrainObject_ABC& GetObject() const;
    void Accept( urban::MotivationsVisitor_ABC& visitor ) const;
    float GetLivingSpace() const;
    //@}

private:
    //!@name Helpers
    //@{
    void InitializeAttributes();
    template < typename T >
    void SendCapacity( sword::UrbanAttributes& msg ) const;
    template < typename T >
    void SendFullStateCapacity( sword::UrbanAttributes& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, float > T_Motivations;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC* object_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UrbanObjectWrapper )

#endif
