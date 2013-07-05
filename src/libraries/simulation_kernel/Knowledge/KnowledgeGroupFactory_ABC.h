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

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
/** @class  KnowledgeGroupFactory_ABC
    @brief  Knowledge group factory declaration
*/
// Created: MGD 2009-10-22
// =============================================================================
class KnowledgeGroupFactory_ABC : private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
             KnowledgeGroupFactory_ABC() {}
    virtual ~KnowledgeGroupFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< MIL_KnowledgeGroup > Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent ) = 0;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int )
    {}
    //@}
};

#endif // __KnowledgeGroupFactory_ABC_h_
