// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_MissionParameter_ABC_h_
#define __MIL_MissionParameter_ABC_h_

#include "MIL_ParameterType_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

namespace sword
{
    class DateTime;
    class Heading;
    class Id;
    class IdList;
    class Location;
    class LocationList;
    class LogMedicalPriorities;
    class MissionObjectiveList;
    class MissionParameter_Value;
    class Path;
    class PathList;
    class PlannedWork;
    class PlannedWorkList;
    class Point;
    class PointList;
    class Polygon;
    class PolygonList;
    class MsgUnitKnowledge;
    class ResourceNetworkElement;
    class ResourceNetworkType;
}

class DEC_Decision_ABC;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_ResourceNetwork;
class MIL_UrbanObject_ABC;
class DEC_Objective;
class MIL_LimaFunction;
class MIL_ParameterType_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;
class PHY_HumanWound;
class PHY_ResourceNetworkType;
class TER_Localisation;

// =============================================================================
/** @class  MIL_MissionParameter_ABC
    @brief  MIL Mission parameter definition
*/
// Created: LDC 2009-04-29
// =============================================================================
class MIL_MissionParameter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_MissionParameter_ABC() {}
    virtual ~MIL_MissionParameter_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;

    typedef std::vector< const PHY_HumanWound* > T_MedicalPriorityVector;
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const = 0;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const = 0;
    virtual bool ToId( int& ) const = 0;
    virtual bool ToNumeric( float& ) const = 0;
    virtual bool ToNumericList( std::vector< float >& ) const = 0;
    virtual bool ToString( std::string& asn ) const = 0;
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& ) const = 0;
    virtual bool ToElement( sword::MissionParameter_Value& ) const = 0;

    // The lifecycle of pointers in all functions below must be handled by MIL_MissionParameter_ABC
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const = 0;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const = 0;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const = 0;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const = 0;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const = 0;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const = 0;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const = 0;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const = 0;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const = 0;
    virtual bool ToNatureAtlas( int& atlas ) const = 0;
    virtual bool ToAutomat( DEC_Decision_ABC*& ) const = 0;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const = 0;
    virtual bool ToAgent( const DEC_Decision_ABC*& ) const = 0;
    virtual bool ToAgentList( std::vector< const DEC_Decision_ABC* >& ) const = 0;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& ) const = 0;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const = 0;
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const = 0;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const = 0;
    virtual bool ToPopulationKnowledge( boost::shared_ptr< DEC_Knowledge_Population >& ) const = 0;
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const = 0;
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const = 0;
    virtual bool ToEquipmentType( const PHY_ComposanteTypePion*& ) const = 0;
    virtual bool ToEquipmentTypeList( std::vector< const PHY_ComposanteTypePion* >& ) const = 0;
    virtual bool ToGDH( float& value ) const = 0;
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const = 0;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const = 0;
    virtual bool ToMaintenancePriorities( T_MaintenancePriorityVector& ) const = 0;
    virtual bool ToMedicalPriorities( T_MedicalPriorityVector& ) const = 0;
    virtual bool ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& ) const = 0;
    virtual bool ToUrbanBlock( MIL_UrbanObject_ABC*& ) const = 0;
    virtual bool ToUrbanBlockList( std::vector< MIL_UrbanObject_ABC* >& ) const = 0;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const = 0;
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const = 0;
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const = 0;
    virtual bool ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& ) const = 0;
    virtual bool ToResourceNetworkNodeList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const = 0;
    virtual bool ToResourceNetworkType( const PHY_ResourceNetworkType*& ) const = 0;
    virtual bool ToResourceNetworkTypeList( std::vector< const PHY_ResourceNetworkType* >& ) const = 0;
    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param ) = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}
};

#endif // __MIL_MissionParameter_ABC_h_
