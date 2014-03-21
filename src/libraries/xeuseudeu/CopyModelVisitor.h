// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_COPY_MODEL_VISITOR_H
#define MODULE_TESTER_COPY_MODEL_VISITOR_H

#include "Model.h"
#include <core/ModelVisitor_ABC.h>

namespace sword
{
namespace test
{

// =============================================================================
/** @class  CopyModelVisitor
    @brief  Copy model visitor
*/
// Created: ABR 2013-08-08
// =============================================================================
class CopyModelVisitor : public core::ModelVisitor_ABC
{
public:

             CopyModelVisitor( Model& model ) : model_( model ) {}
    virtual ~CopyModelVisitor() {}

    virtual void Visit( int64_t value )
    {
        model_ = value;
    }
    virtual void Visit( uint64_t value )
    {
        model_ = value;
    }
    virtual void Visit( double value )
    {
        model_ = value;
    }
    virtual void Visit( const std::string& value )
    {
        model_ = value;
    }
    virtual void Visit( const std::string& key, const core::Model& child )
    {
        Model& newChild = model_[ key ];
        CopyModelVisitor visitor( newChild );
        child.Accept( visitor );
    }
    virtual void Visit( std::size_t key, const core::Model& child )
    {
        Model& newChild = model_[ key ];
        CopyModelVisitor visitor( newChild );
        child.Accept( visitor );
    }
    virtual void Visit( const core::Model& element )
    {
        Model& newElement = model_.AddElement();
        CopyModelVisitor visitor( newElement );
        element.Accept( visitor );
    }
    virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& data )
    {
        model_.SetData( data );
    }

private:
    Model& model_;
};

}
}

#endif // MODULE_TESTER_COPY_MODEL_VISITOR_H
