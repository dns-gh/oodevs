// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DispatcherLoader_h
#define DispatcherLoader_h

#include <tools/Exception.h>
#include <windows.h>
#include <boost/preprocessor/stringize.hpp>
#include <boost/noncopyable.hpp>

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for dispatcher
#endif

#ifdef _DEBUG
# define DEBUG_EXTENSION "-gd"
#else
# define DEBUG_EXTENSION ""
#endif

#define EXTENSION "-" BOOST_PP_STRINGIZE( PLATFORM ) "-mt" DEBUG_EXTENSION ".dll"

namespace dispatcher
{
// =============================================================================
/** @class  DispatcherLoader
    @brief  Dispatcher loader
*/
// Created: SLI 2012-07-18
// =============================================================================
class DispatcherLoader : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef void* ( *T_FacadeCreator )( int argc, char** argv, int maxConnections );
    typedef void ( *T_FacadeDestructor )( void* facade );
    typedef bool ( *T_FacadeUpdator )( void* facade );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    DispatcherLoader( int argc, char** argv, int maxConnections )
        : module_          ( LoadLibrary( "dispatcher" EXTENSION ) )
        , facadeCreator_   ( LoadFunction< T_FacadeCreator >( "CreateDispatcherFacade" ) )
        , facadeDestructor_( LoadFunction< T_FacadeDestructor >( "DestroyDispatcherFacade" ) )
        , facadeUpdator_   ( LoadFunction< T_FacadeUpdator >( "UpdateDispatcherFacade" ) )
        , dispatcher_      ( facadeCreator_( argc, argv, maxConnections ) )
    {
        if( !dispatcher_ )
            throw MASA_EXCEPTION(
                "failed to create dispatcher, check dispatcher.log for details" );
    }
    virtual ~DispatcherLoader()
    {
        facadeDestructor_( dispatcher_ );
        FreeModule( module_ );
    }
    //@}

    //! @name Operations
    //@{
    bool Update()
    {
        return facadeUpdator_( dispatcher_ );
    }
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Function >
    Function LoadFunction( const std::string& name ) const
    {
        if( !module_ )
            throw MASA_EXCEPTION( "failed to load dispatcher" EXTENSION );
        Function fun = reinterpret_cast< Function >( GetProcAddress( module_, name.c_str() ) );
        if( !fun )
            throw MASA_EXCEPTION( "failed to load function '" + name + "' in dispatcher" EXTENSION );
        return fun;
    }
    //@}

private:
    //! @name Member data
    //@{
    HMODULE module_;
    T_FacadeCreator facadeCreator_;
    T_FacadeDestructor facadeDestructor_;
    T_FacadeUpdator facadeUpdator_;
    void* dispatcher_;
    //@}
};

}

#endif // DispatcherLoader_h
