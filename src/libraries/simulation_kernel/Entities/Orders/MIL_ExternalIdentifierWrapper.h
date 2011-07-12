// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ExternalIdentifierWrapper_h_
#define __MIL_ExternalIdentifierWrapper_h_

#include "MIL_MissionParameter_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_ExternalIdentifierWrapper
    @brief  MIL external identifier wrapper
*/
// Created: LGY 2011-06-22
// =============================================================================
class MIL_ExternalIdentifierWrapper : public MIL_MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ExternalIdentifierWrapper();
             MIL_ExternalIdentifierWrapper( boost::shared_ptr< MIL_MissionParameter_ABC > parameter, unsigned int identifier );
    virtual ~MIL_ExternalIdentifierWrapper();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const;
    virtual bool ToId( int& ) const;
    virtual bool ToNumeric( float& ) const;
    virtual bool ToNumericList( std::vector< float >& ) const;
    virtual bool ToString( std::string& asn ) const;
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& ) const;
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToNatureAtlas( int& atlas ) const;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgent( DEC_Decision_ABC*& ) const;
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const;
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const;
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const;
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const;
    virtual bool ToGDH( float& value ) const;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const;
    virtual bool ToUrbanBlock( UrbanObjectWrapper*& ) const;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const;
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork >& ) const;
    virtual bool ToResourceNetworkList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const;
    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param );
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< MIL_MissionParameter_ABC > parameter_;
    unsigned int externalIdentifier_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ExternalIdentifierWrapper )

#endif // __MIL_ExternalIdentifierWrapper_h_
