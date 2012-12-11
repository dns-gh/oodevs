// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_LISTENER_HELPER_H
#define SWORD_LISTENER_HELPER_H

#include <core/ModelListener_ABC.h>
#include <boost/function.hpp>

namespace sword
{
    class Sink;

// =============================================================================
/** @class  ListenerHelper
    @brief  Listener helper
*/
// Created: SLI 2012-03-20
// =============================================================================
class ListenerHelper : private core::ModelListener_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::function< void( const core::Model& model ) > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ListenerHelper( Sink& sink, const core::Model& model, T_Callback changed );
             ListenerHelper( Sink& sink, const core::Model& model, T_Callback changed, T_Callback removed );
    virtual ~ListenerHelper();
    //@}

    //! @name Operations
    //@{
    const core::Model& operator*() const;
    template< typename T >
    const core::Model& operator[]( const T& t ) const
    {
        if( !model_ )
            throw MASA_EXCEPTION( "model has been removed" );
        return (*model_)[ t ];
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyChanged( const core::Model& model );
    virtual void NotifyRemoved( const core::Model& model );
    //@}

private:
    //! @name Member data
    //@{
    Sink& sink_;
    const core::Model* model_;
    T_Callback changed_;
    T_Callback removed_;
    //@}
};

}

#endif // SWORD_LISTENER_HELPER_H
