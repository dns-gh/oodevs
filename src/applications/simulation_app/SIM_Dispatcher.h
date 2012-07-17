// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SIM_Dispatcher_h_
#define __SIM_Dispatcher_h_

// =============================================================================
/** @class  SIM_Dispatcher
    @brief  SIM_Dispatcher
*/
// Created: NLD 2006-10-04
// =============================================================================
class SIM_Dispatcher
{
public:
    //! @name Constructors/Destructor
    //@{
             SIM_Dispatcher( int argc, char** argv, int maxConnections );
    virtual ~SIM_Dispatcher();
    //@}

    //! @name Operations
    //@{
    void Run();
    void Stop();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SIM_Dispatcher( const SIM_Dispatcher& );            //!< Copy constructor
    SIM_Dispatcher& operator=( const SIM_Dispatcher& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef void* ( *T_FacadeCreator )( int argc, char** argv, int maxConnections );
    typedef void ( *T_FacadeDestructor )( void* facade );
    typedef void ( *T_FacadeUpdator )( void* facade );
    //@}

private:
    //! @name Member data
    //@{
    bool running_;
    void* dispatcher_;
    T_FacadeCreator facadeCreator_;
    T_FacadeDestructor facadeDestructor_;
    T_FacadeUpdator facadeUpdator_;
    //@}
};

#endif // __SIM_Dispatcher_h_
