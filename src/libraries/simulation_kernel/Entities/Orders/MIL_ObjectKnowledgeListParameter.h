// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectKnowledgeListParameter_h_
#define __MIL_ObjectKnowledgeListParameter_h_

#include "MIL_BaseParameter.h"
class DEC_Knowledge_Object;
class DEC_KnowledgeResolver_ABC;

// =============================================================================
/** @class  MIL_ObjectKnowledgeListParameter
    @brief  MIL_ObjectKnowledgeListParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_ObjectKnowledgeListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectKnowledgeListParameter( const ASN1T_ObjectKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver );
    explicit MIL_ObjectKnowledgeListParameter( const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& knowledgeObjectList );
    virtual ~MIL_ObjectKnowledgeListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const;
    virtual bool ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectKnowledgeListParameter( const MIL_ObjectKnowledgeListParameter& );            //!< Copy constructor
    MIL_ObjectKnowledgeListParameter& operator=( const MIL_ObjectKnowledgeListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > knowledgeObjectList_;
    //@}
};

#endif // __MIL_ObjectKnowledgeListParameter_h_
