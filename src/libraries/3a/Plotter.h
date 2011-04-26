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

#include "protocol/ClientPublisher_ABC.h"
#include "Functions.h"
#include "Result_ABC.h"
#include "protocol/AarSenders.h"


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
        : publisher_( publisher ), context_( context )
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
        values_.push_back( arg );
    }
    virtual void EndTick() {}

    virtual void Commit( unsigned int skippedFrames, unsigned int firstTick ) const
    {
        aar::PlotResult result;
        result().set_identifier( context_ );
        result().set_error     ( "" );
        for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        {
            if( skippedFrames == 0 )
                result().mutable_values()->Add( float( *it ) );
            else
                --skippedFrames;
        }
        result().set_begin_tick( firstTick );
        result.Send( publisher_, 0 );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Plotter( const Plotter& );            //!< Copy constructor
    Plotter& operator=( const Plotter& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< T > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    int context_;
    T_Values values_;
    //@}
};

#endif // __Plotter_h_
