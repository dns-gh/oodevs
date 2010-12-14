// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "WiseEntity.h"

namespace sword { class KnowledgeGroupCreation; }

class Model;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: SEB 2010-12-13
// =============================================================================
class KnowledgeGroup : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( const Model& model, const sword::KnowledgeGroupCreation& message );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring type_;
    const bool scrambled_;
    const Entity_ABC* party_;
    const Entity_ABC* superior_;
    //@}
};

#endif // __KnowledgeGroup_h_
