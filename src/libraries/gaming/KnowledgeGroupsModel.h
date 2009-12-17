// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupsModel_h_
#define __KnowledgeGroupsModel_h_

#include "tools/Resolver.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
}

class TeamsModel;

// =============================================================================
/** @class  KnowledgeGroupsModel
    @brief  KnowledgeGroupsModel
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupsModel : public tools::Resolver< kernel::KnowledgeGroup_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupsModel();
    virtual ~KnowledgeGroupsModel();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgKnowledgeGroupChangeSuperior& asn );
    void Delete( const ASN1T_MsgKnowledgeGroupDelete& asn );
    void SetType( const ASN1T_MsgKnowledgeGroupSetType& asn );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupsModel( const KnowledgeGroupsModel& );            //!< Copy constructor
    KnowledgeGroupsModel& operator=( const KnowledgeGroupsModel& ); //!< Assignement operator
    //@}
};

#endif // __KnowledgeGroupsModel_h_
