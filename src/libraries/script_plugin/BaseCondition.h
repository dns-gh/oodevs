// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BaseCondition_h_
#define __BaseCondition_h_

#include "Condition_ABC.h"
#include "directia/LuaBinder.h"
#include "directia/ScriptFunctionWrappersTemplates.h"

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  BaseCondition
    @brief  Base condition helper
*/
// Created: AGE 2008-06-17
// =============================================================================
class BaseCondition : public Condition_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BaseCondition();
    virtual ~BaseCondition();
    //@}

    //! @name Operations
    //@{
    virtual void Activate( bool );
    virtual void SetTarget( const directia::WrappedScriptFunction& function );
    //@}

public:
    //! @name Helpers
    //@{
    bool IsActive() const;
    void Trigger();
    template< typename T >
    void Trigger( T p );
    template< typename T1, typename T2 >
    void Trigger( T1 p1, T2 p2 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BaseCondition( const BaseCondition& );            //!< Copy constructor
    BaseCondition& operator=( const BaseCondition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LogError( const std::exception& e );
    //@}

private:
    //! @name Member data
    //@{
    bool active_;
    std::auto_ptr< directia::WrappedScriptFunction > function_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: BaseCondition::Trigger
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
template< typename T >
void BaseCondition::Trigger( T p )
{
    try
    {
        if( active_ && function_.get() )
            (*function_)( p );
    }
    catch( std::exception& e )
    {
        LogError( e );
    }
}

// -----------------------------------------------------------------------------
// Name: BaseCondition::Trigger
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
template< typename T1, typename T2 >
void BaseCondition::Trigger( T1 p1, T2 p2 )
{
    try
    {
        if( active_ && function_.get() )
            (*function_)( p1, p2 );
    }
    catch( std::exception& e )
    {
        LogError( e );
    }
}

}
}

#endif // __BaseCondition_h_
