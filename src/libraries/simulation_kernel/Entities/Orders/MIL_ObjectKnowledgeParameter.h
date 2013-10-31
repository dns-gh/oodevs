// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectKnowledgeParameter_h_
#define __MIL_ObjectKnowledgeParameter_h_

#include "MIL_BaseParameter.h"

class DEC_Knowledge_Object;
class DEC_KnowledgeResolver_ABC;
class MIL_EntityManager_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_ObjectKnowledgeParameter
    @brief  MIL_ObjectKnowledgeParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_ObjectKnowledgeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectKnowledgeParameter();
    explicit MIL_ObjectKnowledgeParameter( boost::shared_ptr< DEC_Knowledge_Object > pObjectKnowledge );
             MIL_ObjectKnowledgeParameter( const sword::Id& asn, const DEC_KnowledgeResolver_ABC& resolver,
                                           const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_ObjectKnowledgeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectKnowledgeParameter( const MIL_ObjectKnowledgeParameter& );            //!< Copy constructor
    MIL_ObjectKnowledgeParameter& operator=( const MIL_ObjectKnowledgeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ObjectKnowledgeParameter )

#endif // __MIL_ObjectKnowledgeParameter_h_
