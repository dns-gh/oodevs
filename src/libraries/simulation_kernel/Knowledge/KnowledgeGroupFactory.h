// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
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
    virtual MIL_KnowledgeGroup& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent = 0 );
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const KnowledgeGroupFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, KnowledgeGroupFactory* role, const unsigned int /*version*/ );
    //@}
};

BOOST_CLASS_EXPORT_KEY( KnowledgeGroupFactory )

#endif // __KnowledgeGroupFactory_h_
