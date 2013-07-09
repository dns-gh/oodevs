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
class KnowledgeGroupFactory_ABC : private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~KnowledgeGroupFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< MIL_KnowledgeGroup > Create( xml::xistream& xis, MIL_Army_ABC& army, boost::shared_ptr< MIL_KnowledgeGroup > parent = boost::shared_ptr< MIL_KnowledgeGroup >() ) = 0;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > Create( MIL_Army_ABC& army ) = 0;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
    
    void Register( const unsigned long& identifier, boost::shared_ptr< MIL_KnowledgeGroup >& element )
    {
        boost::shared_ptr< MIL_KnowledgeGroup >& p = elements_[ identifier ];
        if( p.get() )
            Error( identifier, "already registered" );
        p = element;
    }
    void Remove( const unsigned long& identifier )
    {
        elements_.erase( identifier );
    }

    void Clear()
    {
        elements_.clear();
    }

    virtual boost::shared_ptr< MIL_KnowledgeGroup > Find( const unsigned long& identifier ) const
    {
        CIT_Elements it = elements_.find( identifier );
        if( it != elements_.end() )
            return it->second;
        return boost::shared_ptr< MIL_KnowledgeGroup >();
    }
    
    const std::map< unsigned long, boost::shared_ptr< MIL_KnowledgeGroup > >& GetElements() const
    {
        return elements_;
    }

    unsigned long Count() const
    {
        return (unsigned long)elements_.size();
    }
    //@}

protected:
    //! @name Constructor
    //@{
    KnowledgeGroupFactory_ABC() {};
    //@}

protected:
    //! @name Helpers
    //@{
    void Error( const unsigned long& identifier, const std::string& message ) const
    {
        std::stringstream str;
        str << "MIL_KnowledgeGroup '" << identifier << "' " << message;
        throw std::runtime_error( str.str() );
    }
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, boost::shared_ptr< MIL_KnowledgeGroup > > T_Elements;
    typedef T_Elements::iterator IT_Elements;
    typedef T_Elements::const_iterator CIT_Elements;
    //@}

protected:
    //! @name Member data
    //@{
    T_Elements elements_;
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
