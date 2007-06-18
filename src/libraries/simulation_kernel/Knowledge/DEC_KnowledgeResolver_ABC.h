// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_KnowledgeResolver_ABC_h_
#define __DEC_KnowledgeResolver_ABC_h_

#include "MIL.h"

class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;

// =============================================================================
/** @class  DEC_KnowledgeResolver_ABC
    @brief  DEC_KnowledgeResolver_ABC
*/
// Created: NLD 2006-11-22
// =============================================================================
class DEC_KnowledgeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeResolver_ABC();
    virtual ~DEC_KnowledgeResolver_ABC();
    //@}

    //! @name Operations
    //@{
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const = 0;
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent ( const DIA_Variable_ABC&      dia ) const = 0;
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent (       uint                   nID ) const = 0;

    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const = 0;
    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject( const DIA_Variable_ABC&      dia ) const = 0;
    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject(       uint                   nID ) const = 0;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const DIA_Variable_ABC&          dia ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_KnowledgeResolver_ABC( const DEC_KnowledgeResolver_ABC& );            //!< Copy constructor
    DEC_KnowledgeResolver_ABC& operator=( const DEC_KnowledgeResolver_ABC& ); //!< Assignement operator
    //@}

private:
};

#endif // __DEC_KnowledgeResolver_ABC_h_
