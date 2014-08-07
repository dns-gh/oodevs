// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_NAMED_CHILDREN_VISITOR_H
#define CORE_NAMED_CHILDREN_VISITOR_H

#include "ModelVisitor_ABC.h"

namespace core
{
    class Model;

// =============================================================================
/** @class  NamedChildrenVisitor
    @brief  String children visitor
*/
// Created: SLI 2012-07-18
// =============================================================================
class NamedChildrenVisitor : private ModelVisitor_ABC
{
public:
    //! @name Types
    //@{
    typedef std::function< void( const std::string&, const Model& ) > T_Functor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             NamedChildrenVisitor( const Model& parent, const T_Functor& functor );
    virtual ~NamedChildrenVisitor();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Visit( int64_t value );
    virtual void Visit( uint64_t value );
    virtual void Visit( double value );
    virtual void Visit( const std::string& value );
    virtual void Visit( const std::string& key, const Model& child );
    virtual void Visit( std::size_t key, const Model& child );
    virtual void Visit( const Model& element );
    virtual void Visit( const boost::shared_ptr< UserData_ABC >& data );
    //@}

private:
    //! @name Member data
    //@{
    T_Functor functor_;
    //@}
};

}

#endif // CORE_NAMED_CHILDREN_VISITOR_H
