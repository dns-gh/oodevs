// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#define SWORD_BRAIN_MEMBER_READ_PARAM(z, n, d) \
    BOOST_PP_COMMA_IF(n) Read< P##n >( vm, BOOST_PP_INC(BOOST_PP_INC(n)) )

    template< typename T, typename R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    struct BOOST_PP_CAT(Reader, BRAIN_NUM_ARGS)
    {
        template< typename Function >
        int DoRead( directia::tools::VirtualMachine_ABC& vm, const Function& f ) const
        {
            using namespace directia::tools::binders;
            R result = f( Read< T& >( vm, 1 ) BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS)
                BOOST_PP_REPEAT(BRAIN_NUM_ARGS, SWORD_BRAIN_MEMBER_READ_PARAM, BOOST_PP_EMPTY) );
            Write< R >( vm, result );
            return 1;
        }
    };

    template< typename T BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    struct BOOST_PP_CAT(Reader, BRAIN_NUM_ARGS)< T, void BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) >
    {
        template< typename Function >
        int DoRead( directia::tools::VirtualMachine_ABC& vm, const Function& f ) const
        {
            using namespace directia::tools::binders;
            f( Read< T& >( vm, 1 ) BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS)
                BOOST_PP_REPEAT(BRAIN_NUM_ARGS, SWORD_BRAIN_MEMBER_READ_PARAM, BOOST_PP_EMPTY) );
            return 0;
        }
    };

#undef SWORD_BRAIN_MEMBER_READ_PARAM

#define SWORD_BRAIN_MEMBER_REGISTER_PARAM(z, n, d) \
    RegisterType< P##n >( vm );

    template< typename T, typename R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    struct BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)
        : public directia::tools::binders::ScriptFunction_ABC
        , BOOST_PP_CAT(Reader, BRAIN_NUM_ARGS)< T, R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) >
    {
        typedef boost::function< R( T& BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) > Function;
        BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)( directia::tools::VirtualMachine_ABC& vm, const char* const name, Function f )
            : f_( f )
        {
            using namespace directia::tools::binders;
            RegisterType< T& >( vm );
            RegisterType< R >( vm );
            BOOST_PP_REPEAT(BRAIN_NUM_ARGS, SWORD_BRAIN_MEMBER_REGISTER_PARAM, BOOST_PP_EMPTY)
            RegisterMember< T& >( vm, name );
        }
        int operator()( directia::tools::VirtualMachine_ABC& vm ) const
        {
            using namespace directia::tools::binders;
            impl::CheckSize( vm, BOOST_PP_INC(BRAIN_NUM_ARGS) );
            return DoRead( vm, f_ );
        }
        Function f_;
    };

#undef SWORD_BRAIN_MEMBER_REGISTER_PARAM

    template< typename R, typename T BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    void RegisterMethod( const char* const name, R(T::*method)( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) )
    {
        new ( directia::tools::binders::ScriptFunction_ABC::Alloc( brain_->vm_,
                sizeof( BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)< T, R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) > ) ) )
            BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)< T, R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) >(
                    brain_->vm_, name,
                        ProfilerProxy< R( T& BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) >( profilers_[ name ],
                            boost::bind( method, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_INC(BRAIN_NUM_ARGS)), _) ) ) );
    }

    template< typename R, typename T BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    void RegisterMethod( const char* const name, R(T::*method)( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) const )
    {
        new ( directia::tools::binders::ScriptFunction_ABC::Alloc( brain_->vm_,
                sizeof( BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)< T, R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) > ) ) )
            BOOST_PP_CAT(ScriptMemberFunction, BRAIN_NUM_ARGS)< T, R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) >(
                    brain_->vm_, name,
                        ProfilerProxy< R( T& BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) >( profilers_[ name ],
                            boost::bind( method, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_INC(BRAIN_NUM_ARGS)), _) ) ) );
    }

    template< typename R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    void RegisterFunction( const char* const name, R(*function)( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) )
    {
        RegisterFunction( name, boost::function< R( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, P) ) >( function ) );
    }

    template< typename Signature >
    struct ProfilerProxy;

    template< typename R BOOST_PP_COMMA_IF(BRAIN_NUM_ARGS) BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, typename P) >
    struct ProfilerProxy< R( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS,P) ) >
    {
        typedef typename boost::function< R( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS,P) ) > function_type;
        typedef typename function_type::result_type result_type;
        ProfilerProxy( MT_Profiler& profiler, const function_type& f )
            : profiler_( &profiler )
            , f_       ( f )
        {}
        R operator()( BOOST_PP_ENUM_BINARY_PARAMS(BRAIN_NUM_ARGS, P, t) ) const
        {
            MT_ProfilerGuard guard( *profiler_ );
            return f_( BOOST_PP_ENUM_PARAMS(BRAIN_NUM_ARGS, t) );
        }
        MT_Profiler* profiler_;
        function_type f_;
    };
