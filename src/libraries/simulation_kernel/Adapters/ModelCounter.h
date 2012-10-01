// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_MODEL_COUNTER_H
#define SWORD_MODEL_COUNTER_H

#include <core/ModelVisitor_ABC.h>

namespace sword
{
// =============================================================================
/** @class  ModelCounter
    @brief  ModelCounter
*/
// Created: MCO 2012-09-28
// =============================================================================
class ModelCounter : public core::ModelVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ModelCounter();
    virtual ~ModelCounter();
    //@}

    //! @name Operations
    //@{
    virtual void Visit( int64_t value );
    virtual void Visit( uint64_t value );
    virtual void Visit( double value );
    virtual void Visit( const std::string& value );

    virtual void Visit( const std::string& key, const core::Model& child );
    virtual void Visit( unsigned int key, const core::Model& child );

    virtual void Visit( const core::Model& element );

    virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& data );

    virtual void MarkForRemove();
    //@}

    //! @name Member data
    //@{
    unsigned long count_;
    //@}
};

}

#endif // SWORD_MODEL_COUNTER_H
