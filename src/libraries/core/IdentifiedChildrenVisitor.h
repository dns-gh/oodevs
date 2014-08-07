// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_IDENTIFIED_CHILDREN_VISITOR_H
#define CORE_IDENTIFIED_CHILDREN_VISITOR_H

#include "ModelVisitor_ABC.h"

namespace core
{
    class Model;

// =============================================================================
/** @class  IdentifiedChildrenVisitor
    @brief  Identified children visitor
*/
// Created: MCO 2012-09-25
// =============================================================================
class IdentifiedChildrenVisitor : private ModelVisitor_ABC
{
public:
    //! @name Types
    //@{
    typedef std::function< void( std::size_t, const Model& ) > T_Functor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             IdentifiedChildrenVisitor( const Model& parent, const T_Functor& functor );
    virtual ~IdentifiedChildrenVisitor();
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

#endif // CORE_IDENTIFIED_CHILDREN_VISITOR_H
