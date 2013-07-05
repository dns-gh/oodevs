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
#include <boost/serialization/export.hpp>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  FormationFactory
    @brief  Formation factory
*/
// Created: MGD 2009-10-22
// =============================================================================
class KnowledgeGroupFactory : public KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupFactory();
    virtual ~KnowledgeGroupFactory();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< MIL_KnowledgeGroup > Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent );
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
};

BOOST_CLASS_EXPORT_KEY( KnowledgeGroupFactory )

#endif // __KnowledgeGroupFactory_h_
