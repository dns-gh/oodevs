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

class MIL_ObjectBuilder_ABC;

namespace sword
{
    class UrbanAttributes;
}

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  UrbanObjectWrapper
    @brief  UrbanObjectWrapper
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

    //! @name HLA
    //@{
    virtual HLA_Object_ABC* GetHLAView() const;
    virtual void            SetHLAView( HLA_Object_ABC& view );

    virtual void    Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    virtual void    Serialize  ( HLA_UpdateFunctor& functor ) const;
    //@}

    sword::MagicActionAck_ErrorCode OnUpdateStructuralState( int state );
    //! @name Network
    //@{
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );
    virtual sword::ObjectMagicActionAck_ErrorCode OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );

    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    virtual void UpdateState();

    template < typename T >
    void SendCapacity( sword::UrbanAttributes& msg );
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int              GetID() const;
    unsigned int                      GetUrbanId() const;
    const std::string&                GetName() const;
    const urban::TerrainObject_ABC&   GetObject();
    const urban::TerrainObject_ABC&   GetObject() const;
    //@}

protected:
    //!@name Helpers
    //@{
    void InitializeAttributes();
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC* object_;
    unsigned int                    id_;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* pView_;
    //@}

public:

    typedef std::map< const urban::TerrainObject_ABC*, UrbanObjectWrapper*  >     T_ObjectMap;
    typedef T_ObjectMap::iterator                                                IT_ObjectMap;
    typedef T_ObjectMap::const_iterator                                         CIT_ObjectMap;

    static T_ObjectMap objectMap_;

    static UrbanObjectWrapper& GetWrapperObject( const urban::TerrainObject_ABC& object );
};

BOOST_CLASS_EXPORT_KEY( UrbanObjectWrapper )

#endif
