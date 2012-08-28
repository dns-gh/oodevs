// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanObject_h_
#define __MIL_UrbanObject_h_

#include "MIL_UrbanObject_ABC.h"


class MIL_ObjectBuilder_ABC;

namespace sword
{
    class UrbanAttributes;
}

class MIL_LivingArea;
class MIL_UrbanObject_ABC;
class MIL_UrbanMotivationsVisitor_ABC;

// =============================================================================
/** @class  MIL_UrbanObject
    @brief  MIL_UrbanObject
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanObject : public MIL_UrbanObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanObject( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_UrbanObject_ABC* parent = 0 );
             MIL_UrbanObject();
    virtual ~MIL_UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetUrbanId() const;
    virtual const std::string& GetName() const;
    virtual MIL_UrbanObject_ABC* GetParent() const;
    virtual void Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const;
    virtual void ComputeConvexHull();
    virtual void GetUrbanObjectLeaves( std::vector< const MIL_UrbanObject_ABC* >& leaves ) const;
    virtual bool HasChild() const;
    virtual float GetLivingSpace() const;
    virtual float ComputeComplexity() const;
    virtual const std::string& GetInfrastructure() const;
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
    virtual float GetStructuralHeight() const;
    virtual float GetStructuralState() const;
    virtual const std::string& GetMaterial() const;
    virtual double GetOccupation() const;
    virtual float GetHeight() const;
    virtual bool HasArchitecture() const;
    virtual unsigned int GetTotalInhabitants() const;
    virtual const std::vector< boost::shared_ptr< MT_Vector2D > >& ComputeLocalisationsInsideBlock() const;
    virtual double GetTrafficability() const;
    virtual bool IsBlock() const;
    virtual bool HasParent() const;
    //@}

    //! @name Inhabitants
    //@{
    virtual void UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number );
    virtual unsigned int GetTotalInhabitantsForMotivation( const std::string& motivation ) const;
    virtual void AddLivingArea( MIL_LivingArea& livingArea );
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
    unsigned long nUrbanId_;
    std::string name_;
    MIL_UrbanObject_ABC* parent_;
    std::string infrastructure_;
    T_Inhabitants inhabitants_;
    T_LivingAreas livingAreas_;
    mutable std::vector< boost::shared_ptr< MT_Vector2D > > strechedArea_;
    static const float stretchOffset_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_UrbanObject )

#endif // __MIL_UrbanObject_h_
