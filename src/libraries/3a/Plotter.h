// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Plotter_h_
#define __Plotter_h_

#include "Functions.h"
#include "Result_ABC.h"
#include <vector>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

void SendPlotResult( int context, const std::vector< float >& values,
        dispatcher::ClientPublisher_ABC& pub, unsigned int skippedFrames,
        unsigned int firstTick );

// =============================================================================
/** @class  Plotter
    @brief  Plotter
*/
// Created: AGE 2007-09-12
// =============================================================================
template< typename K, typename T >
class Plotter : public Function1_ABC< K, T >
              , public Result_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    Plotter( dispatcher::ClientPublisher_ABC& publisher, int context )
        : publisher_( publisher )
        , context_  ( context )
    {
        // NOTHING
    }
    virtual ~Plotter()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const
    {
        return "Plotter";
    }
    virtual void BeginTick() {}
    virtual void SetKey( const K& ) {}
    virtual void Apply( const T& arg )
    {
        values_.push_back( static_cast< float >( arg ));
    }
    virtual void EndTick() {}
    virtual void Commit( unsigned int skippedFrames, unsigned int firstTick ) const
    {
        return SendPlotResult( context_, values_, publisher_, skippedFrames, firstTick );
    }
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    int context_;
    std::vector< float > values_;
    //@}
};

#endif // __Plotter_h_
