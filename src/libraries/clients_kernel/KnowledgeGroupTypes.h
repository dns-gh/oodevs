// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupTypes_h_
#define __KnowledgeGroupTypes_h_

#include "Resolver.h"

namespace kernel
{
    class KnowledgeGroupType;

// =============================================================================
/** @class  KnowledgeGroupTypes
    @brief  KnowledgeGroupTypes
*/
// Created: SBO 2006-10-27
// =============================================================================
class KnowledgeGroupTypes : public kernel::Resolver< KnowledgeGroupType, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupTypes();
    virtual ~KnowledgeGroupTypes();
    //@}

    //! @name Operations
    //@{
    KnowledgeGroupType& GetDefault() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupTypes( const KnowledgeGroupTypes& );            //!< Copy constructor
    KnowledgeGroupTypes& operator=( const KnowledgeGroupTypes& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    KnowledgeGroupType* default_;
    //@}
};

#endif // __KnowledgeGroupTypes_h_
