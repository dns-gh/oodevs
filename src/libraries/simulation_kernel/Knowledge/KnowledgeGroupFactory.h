// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_h_
#define __KnowledgeGroupFactory_h_

#include "KnowledgeGroupFactory_ABC.h"

// =============================================================================
/** @class  FormationFactory
@brief  FormationFactory
*/
// Created: MGD 2009-10-22
// =============================================================================
class KnowledgeGroupFactory : public KnowledgeGroupFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeGroupFactory();
    virtual ~KnowledgeGroupFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_KnowledgeGroup& Create( xml::xistream& xis, MIL_Army& army, MIL_KnowledgeGroup* parent = 0 );
    //@}

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const KnowledgeGroupFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, KnowledgeGroupFactory* role, const unsigned int /*version*/ );
    //@}
};

#endif // __KnowledgeGroupFactory_h_
