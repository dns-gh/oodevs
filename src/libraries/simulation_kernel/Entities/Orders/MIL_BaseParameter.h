// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_BaseParameter_h_
#define __MIL_BaseParameter_h_

#include "MIL_MissionParameter_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_BaseParameter
    @brief  MIL_BaseParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_BaseParameter : public MIL_MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_BaseParameter();
    virtual ~MIL_BaseParameter();
    //@}

    //! @name Conversions
    //@{
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const;
    virtual bool ToBool( bool& ) const;
    virtual bool ToId( int& ) const;
    virtual bool ToString( std::string& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToNatureAtlas( int& atlas ) const;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgent( const DEC_Decision_ABC*& ) const;
    virtual bool ToAgentList( std::vector< const DEC_Decision_ABC* >& ) const;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const;
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const;
    virtual bool ToPopulationKnowledge( boost::shared_ptr< DEC_Knowledge_Population >& ) const;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const;
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const;
    virtual bool ToEquipmentTypeList( std::vector< const PHY_ComposanteTypePion* >& ) const;
    virtual bool ToGDH( float& value ) const;
    virtual bool ToNumeric( float& value ) const;
    virtual bool ToNumericList( std::vector< float >& value ) const;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const;
    virtual bool ToUrbanBlock( MIL_UrbanObject_ABC*& ) const;
    virtual bool ToUrbanBlockList( std::vector< MIL_UrbanObject_ABC* >& ) const;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const;
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& ) const;
    virtual bool ToResourceNetworkNodeList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const;
    virtual bool ToResourceNetworkType( const PHY_ResourceNetworkType*& ) const;
    virtual bool ToResourceNetworkTypeList( std::vector< const PHY_ResourceNetworkType* >& ) const;
    virtual bool ToItinerary( sword::Pathfind& ) const;

    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param );
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_BaseParameter )

#endif // __MIL_BaseParameter_h_
