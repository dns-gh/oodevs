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
#include <tools/Map.h>
#include "Tools/MIL_IDManager.h"
#include <map>

namespace sword
{
    class UrbanAttributes;
}

namespace xml
{
    class xistream;
}

class MIL_LivingArea;
class MIL_UrbanObject_ABC;

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
             MIL_UrbanObject( unsigned long id, const std::string& name, const MIL_ObjectType_ABC& type,
                              MIL_UrbanObject_ABC* parent = 0 );
             MIL_UrbanObject();
    virtual ~MIL_UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetUrbanId() const;
    virtual const std::string& GetName() const;
    virtual MIL_UrbanObject_ABC* GetParent() const;
    virtual void ComputeConvexHull();
    virtual void GetUrbanBlocks( std::vector< const MIL_UrbanObject_ABC* >& blocks ) const;
    virtual float GetLivingSpace() const;
    void ReadData( xml::xistream& xis );
    virtual int GetFloorNumber() const;
    virtual double GetOccupation() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    virtual void WriteUrban( xml::xostream& xos ) const;

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}
    
    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_KnowledgeGroup& group );
    //@}
    
    //! @name Network
    //@{
    virtual void OnUpdate( const sword::MissionParameters& params );
    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    virtual void UpdateState();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetTotalInhabitants() const;
    virtual TER_Polygon GetScaledLocation( double distance ) const;
    virtual const std::vector< boost::shared_ptr< MT_Vector2D > >& ComputeLocalisationsInsideBlock() const;
    virtual bool IsBlock() const;
    //@}

    //! @name Inhabitants
    //@{
    virtual void UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number );
    virtual unsigned int GetTotalInhabitantsForMotivation( const std::string& motivation ) const;
    virtual void AddLivingArea( MIL_LivingArea& livingArea );
    //@}

    virtual bool CanInteractWith      ( const MIL_Agent_ABC& agent ) const;
    virtual bool CanInteractWith      ( const MIL_Population& population ) const;
    virtual bool CanInteractWithEntity() const;
    virtual void ApplyIndirectFire( const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation, MIL_Army_ABC* army );
    virtual void ApplyDirectFire() const;
    virtual void ApplyDestruction( const TER_Localisation& attritionSurface, const PHY_UrbanAttritionData& attrition );
    virtual void PreprocessAgent         ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside ( MIL_PopulationElement_ABC& population );
    virtual void ApplyStructuralState( float structuralState ) const;
    virtual bool CanCollideWithEntity() const;

private:
    //!@name Helpers
    //@{
    void ReadLocalisation( xml::xistream& xis );
    void ReadColor( xml::xistream& xis );
    void ReadInfrastructure( xml::xistream& xis );
    void ReadPhysical( xml::xistream& xis );
    void ReadResourceNetworks( xml::xistream& xis );
    void ReadStructuralState( xml::xistream& xis );
    void ReadPoint( xml::xistream& xis, T_PointVector& vector );
    void WriteLocalisation( xml::xostream& xos ) const;
    void WriteColor( xml::xostream& xos ) const;
    template< typename T >
    void WriteCapacity( xml::xostream& xos ) const;
    void WritePointVector( xml::xostream& xos, const T_PointVector& vector ) const;
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
    typedef std::map< std::string, unsigned int > T_Motivations;
    typedef tools::Map< MIL_LivingArea*, T_Motivations > T_Inhabitants;

    struct UrbanColor
    {
        UrbanColor()
            : red_( 200 )
            , green_( 200 )
            , blue_( 200 )
            , alpha_( 0.7f )
        {}
        unsigned short red_;
        unsigned short green_;
        unsigned short blue_;
        float alpha_;
    };
    //@}

private:
    //! @name Member data
    //@{
    unsigned long nUrbanId_;
    std::string name_;
    MIL_UrbanObject_ABC* parent_;
    UrbanColor color_;
    T_Inhabitants inhabitants_;
    T_LivingAreas livingAreas_;
    mutable float complexity_;
    mutable float livingSpace_;
    mutable std::vector< boost::shared_ptr< MT_Vector2D > > stretchedArea_;
    typedef std::list< double > T_LastLocations;
    mutable T_LastLocations lastUsedScaledLocations_;
    mutable std::map< double, std::pair< TER_Polygon, T_LastLocations::iterator > > scaledLocations_;
    static const float stretchOffset_;
    static MIL_IDManager idManager_;
    //@}

public:
    //! @name Member data
    //@{
    static size_t maxScaledLocationsNumber_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_UrbanObject )

#endif // __MIL_UrbanObject_h_
