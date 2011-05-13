// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ListParameter_h_
#define __MIL_ListParameter_h_

#include "MIL_BaseParameter.h"
#include "protocol/Protocol.h"

class DEC_KnowledgeResolver_ABC;

// =============================================================================
/** @class  MIL_ListParameter
    @brief  MIL_ListParameter
*/
// Created: LDC 2010-09-21
// =============================================================================
class MIL_ListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list );
             MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& paramList );
    virtual ~MIL_ListParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;

    virtual bool ToElement( sword::MissionParameter_Value& ) const;
    virtual bool ToList( ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& message ) const;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const;

    virtual bool ToNumericList( std::vector< float >& result ) const;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& result ) const;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    virtual bool ToAutomatList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgentList( std::vector< DEC_Decision_ABC* >& ) const;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const;
    virtual bool ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& ) const;
    virtual bool ToDotationTypeList( std::vector< const PHY_DotationCategory* >& ) const;
    virtual bool ToResourceNetworkList( std::vector< boost::shared_ptr< DEC_ResourceNetwork > >& ) const;

    virtual void Append( boost::shared_ptr< MIL_MissionParameter_ABC > param );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ListParameter( const MIL_ListParameter& );            //!< Copy constructor
    MIL_ListParameter& operator=( const MIL_ListParameter& ); //!< Assignment operator
    //@}

    typedef std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > T_ParameterList;
    typedef T_ParameterList::const_iterator                          CIT_ParameterList;

    T_ParameterList list_;
    const DEC_KnowledgeResolver_ABC& resolver_;
};

#endif // __MIL_ListParameter_h_
