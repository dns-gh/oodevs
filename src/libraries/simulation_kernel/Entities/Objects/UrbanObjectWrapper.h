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
    class UrbanAttributes;
}

namespace urban
{
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

    //! @name Construction
    //@{
    virtual void UpdateLocalisation( const TER_Localisation& location );
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
    virtual sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );
    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    virtual void UpdateState();
    //@}

    //! @name Accessors
    //@{
    bool Is( const urban::TerrainObject_ABC& object ) const;
    const std::string& GetName() const;
    bool HasChild() const;
    float ComputeComplexity() const;
    float GetHeight() const;
    float GetStructuralHeight() const;
    float GetOccupation() const;
    float GetTrafficability() const;
    const urban::TerrainObject_ABC& GetObject() const;  // $$$$ _RC_ LGY 2011-02-18: à supprimer
    void Accept( urban::MotivationsVisitor_ABC& visitor ) const;
    float GetLivingSpace() const;
    const std::vector< boost::shared_ptr< MT_Vector2D > >& ComputeLocalisationsInsideBlock();
    //@}

    //! @name Inhabitants
    //@{
    void UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number );
    unsigned int GetTotalInhabitants() const;
    unsigned int GetTotalInhabitantsForMotivation( const std::string& motivation ) const;
    void AddLivingArea( MIL_LivingArea& livingArea );
    //@}

private:
    //!@name Helpers
    //@{
    void InitializeAttributes();
    template < typename T >
    void SendCapacity( sword::UrbanAttributes& msg ) const;
    template < typename T >
    void SendFullStateCapacity( sword::UrbanAttributes& msg ) const;
    void OnReceiveSetAlerted( const sword::MissionParameter_Value& attribute );
    void OnReceiveSetConfined( const sword::MissionParameter_Value& attribute );
    void OnReceiveSetEvacuated( const sword::MissionParameter_Value& attribute );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_LivingArea* > T_LivingAreas;
    typedef T_LivingAreas::iterator       IT_LivingAreas;

    typedef std::map< std::string, unsigned int > T_Motivations;
    typedef T_Motivations::iterator              IT_Motivations;
    typedef T_Motivations::const_iterator       CIT_Motivations;

    typedef std::map< MIL_LivingArea*, T_Motivations > T_Inhabitants;
    typedef T_Inhabitants::const_iterator            CIT_Inhabitants;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC* object_;
    T_Inhabitants inhabitants_;
    T_LivingAreas livingAreas_;
    std::vector< boost::shared_ptr< MT_Vector2D > > strechedArea_;
    static const float stretchOffset_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UrbanObjectWrapper )

#endif // __UrbanObjectWrapper_h_
