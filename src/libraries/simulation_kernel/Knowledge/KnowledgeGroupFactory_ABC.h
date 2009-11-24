// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_ABC_h_
#define __KnowledgeGroupFactory_ABC_h_

#include "tools/Resolver.h"

class MIL_Army;
class MIL_KnowledgeGroup;

namespace xml
{
    class xistream;
}
// =============================================================================
/** @class  FormationFactory_ABC
@brief  FormationFactory_ABC
*/
// Created: MGD 2009-10-22
// =============================================================================
class KnowledgeGroupFactory_ABC : public tools::Resolver< MIL_KnowledgeGroup >
                                , private boost::noncopyable
{

public:
    //! @name Destructor
    //@{
    virtual ~KnowledgeGroupFactory_ABC() { DeleteAll(); };
    //@}

    //! @name Operations
    //@{
    virtual MIL_KnowledgeGroup& Create( xml::xistream& xis, MIL_Army& army, MIL_KnowledgeGroup* parent = 0 ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const uint );
    //@}

protected:
    //! @name Constructor
    //@{
    KnowledgeGroupFactory_ABC() {};
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void FormationFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void KnowledgeGroupFactory_ABC::serialize( Archive& file, const uint )
{
    file & elements_;
}

#endif // __KnowledgeGroupFactory_ABC_h_
