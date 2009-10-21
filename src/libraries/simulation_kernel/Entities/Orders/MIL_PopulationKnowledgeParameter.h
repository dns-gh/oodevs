// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PopulationKnowledgeParameter_h_
#define __MIL_PopulationKnowledgeParameter_h_

#include "MIL_BaseParameter.h"
class DEC_Knowledge_Object;
class DEC_KnowledgeResolver_ABC;

// =============================================================================
/** @class  MIL_PopulationKnowledgeParameter
    @brief  MIL_PopulationKnowledgeParameter
*/
// Created: LDC 2009-06-04
// =============================================================================
class MIL_PopulationKnowledgeParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_PopulationKnowledgeParameter( DEC_Knowledge_Population* pKnowledge );
             MIL_PopulationKnowledgeParameter( const ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );
    virtual ~MIL_PopulationKnowledgeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPopulationKnowledge( ASN1T_PopulationKnowledge& asn ) const;
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PopulationKnowledgeParameter( const MIL_PopulationKnowledgeParameter& );            //!< Copy constructor
    MIL_PopulationKnowledgeParameter& operator=( const MIL_PopulationKnowledgeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DEC_Knowledge_Population* pKnowledgePopulation_;
    //@}
};

#endif // __MIL_PopulationKnowledgeParameter_h_
