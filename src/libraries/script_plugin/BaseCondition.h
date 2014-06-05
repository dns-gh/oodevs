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
#include "directia/tools/binders/ScriptRef.h"

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
    virtual void SetTarget( const directia::tools::binders::ScriptRef& function );
    //@}

    //! @name Helpers
    //@{
    bool IsActive() const;

    void Trigger();
    template< typename T > void Trigger( T p )
    {
        try
        {
            if( active_ && function_.get() )
                (*function_)( p );
        }
        catch( const std::exception& e )
        {
            LogError( e );
        }
    }
    template< typename T1, typename T2 > void Trigger( T1 p1, T2 p2 )
    {
        try
        {
            if( active_ && function_.get() )
                (*function_)( p1, p2 );
        }
        catch( const std::exception& e )
        {
            LogError( e );
        }
    }
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
    std::unique_ptr< directia::tools::binders::ScriptRef > function_;
    //@}
};

}
}

#endif // __BaseCondition_h_
