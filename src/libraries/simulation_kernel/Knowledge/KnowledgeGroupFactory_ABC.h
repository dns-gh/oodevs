// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_ABC_h_
#define __KnowledgeGroupFactory_ABC_h_

#include "tools/Resolver.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

class MIL_Army;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  KnowledgeGroupFactory_ABC
    @brief  Knowledge group factory declaration
*/
// Created: MGD 2009-10-22
// =============================================================================
class KnowledgeGroupFactory_ABC : public tools::Resolver< MIL_KnowledgeGroup >
                                , private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~KnowledgeGroupFactory_ABC()
    {
        DeleteAll();
    };
    //@}

    //! @name Operations
    //@{
    virtual MIL_KnowledgeGroup& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent = 0 ) = 0;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

protected:
    //! @name Constructor
    //@{
    KnowledgeGroupFactory_ABC() {};
    //@}
};

// BOOST_SERIALIZATION_ASSUME_ABSTRACT should be used for this
// but it seems to be buggy : inherits boost::is_abstract<T> instead of boost::true_type
namespace boost
{
namespace serialization
{
    template<>
    struct is_abstract< const KnowledgeGroupFactory_ABC >
        : boost::true_type
    {
        // NOTHING
    };
} // namespace serialization
}

#endif // __KnowledgeGroupFactory_ABC_h_
