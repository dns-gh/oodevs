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
#include <geometry/Types.h>

class MIL_ObjectBuilder_ABC;

namespace sword
{
    enum UrbanMagicActionAck_ErrorCode;
    class UrbanAttributes;
    class UrbanMagicAction_Infrastructure;
}

namespace urban
{
    class Architecture;
    class TerrainObject_ABC;
    class MotivationsVisitor_ABC;
}

class MIL_LivingArea;

// =============================================================================
/** @class  UrbanObjectWrapper
    @brief  Urban object wrapper
*/
// Created: SLG 2010-06-18
// =============================================================================
class UrbanObjectWrapper : public MIL_Object // $$$$ _RC_ LGY 2011-02-18: urban::TerrainObject_ABC (proxy)
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

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team );
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group );
    //@}

    //! @name Network
    //@{
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
    bool HasChild() const;
    const geometry::Polygon2f* GetFootprint() const;
    float ComputeComplexity() const;
    const urban::Architecture* GetArchitecture() const;
    const urban::TerrainObject_ABC& GetObject();  // $$$$ _RC_ LGY 2011-02-18: à supprimer
    const urban::TerrainObject_ABC& GetObject() const;  // $$$$ _RC_ LGY 2011-02-18: à supprimer
    void Accept( urban::MotivationsVisitor_ABC& visitor ) const;
    float GetLivingSpace() const;
    //@}

    //! @name Inhabitants
    //@{
    void UpdateInhabitants( const MIL_LivingArea& livingArea, unsigned int number );
    unsigned int GetTotalInhabitants() const;
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
    typedef std::map< const MIL_LivingArea*, unsigned int > T_Inhabitants;
    typedef T_Inhabitants::const_iterator                 CIT_Inhabitants;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC* object_;
    T_Inhabitants inhabitants_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UrbanObjectWrapper )

#endif
