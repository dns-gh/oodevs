// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_MODEL_BUILDER_H
#define CORE_MODEL_BUILDER_H

#include "Model.h"
#include "UserData.h"
#include <boost/shared_ptr.hpp>

namespace core
{
    template< typename Model >
    struct ModelBuilder
    {
        ModelBuilder()
            : model_( new Model() )
        {}

        template< typename Value >
        explicit ModelBuilder( Value value )
            : model_( new Model() )
        {
            *model_ = value;
        }

        explicit ModelBuilder( boost::shared_ptr< UserData_ABC > data )
            : model_( new Model() )
        {
            model_->SetData( data );
        }

        template< typename Key, typename Value >
        ModelBuilder( Key key, Value value )
            : model_( new Model() )
        {
            (*model_)[ key ] = value;
        }

        template< typename Key >
        ModelBuilder( Key key, boost::shared_ptr< UserData_ABC > data )
            : model_( new Model() )
        {
            (*model_)[ key ].SetData( data );
        }

        template< typename Key >
        ModelBuilder( Key key, const ModelBuilder& builder )
            : model_( new Model() )
        {
            (*model_)[ key ] = *builder.model_;
        }

        template< typename Key, typename Value >
        ModelBuilder& operator()( Key key, Value value )
        {
            (*model_)[ key ] = value;
            return *this;
        }

        template< typename Key >
        ModelBuilder& operator()( Key key, const ModelBuilder& builder )
        {
            (*model_)[ key ] = *builder.model_;
            return *this;
        }

        template< typename Key >
        ModelBuilder& operator()( Key key, boost::shared_ptr< UserData_ABC > data )
        {
            (*model_)[ key ].SetData( data );
            return *this;
        }

        ModelBuilder& operator[]( const ModelBuilder& builder )
        {
            model_->AddElement() = *builder.model_;
            return *this;
        }

        ModelBuilder& operator[]( boost::shared_ptr< UserData_ABC > data )
        {
            model_->AddElement().SetData( data );
            return *this;
        }

        operator const Model&() const
        {
            return *model_;
        }

        boost::shared_ptr< Model > model_;
    };
}

#endif // CORE_MODEL_BUILDER_H
