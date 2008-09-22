// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ProcessList_h_
#define __ProcessList_h_

#include <boost/ptr_container/ptr_map.hpp>

namespace tools
{

// =============================================================================
/** @class  ProcessList
    @brief  ProcessList
*/
// Created: RDS 2008-08-18
// =============================================================================
class ProcessList
{

public:
    //! @name Constructors/Destructor
    //@{
             ProcessList();
    virtual ~ProcessList();
    //@}

    //! @name Operations
    //@{
    bool         Contains( const std::string& procName ) const; 
    unsigned int KillAll ( const std::string& procName ) ;  
    //@}

private:

    //! @name 
    //@{
    class ProcInfo ;                                      //!< obscure process information class to hide OS imaplementation 
    typedef boost::ptr_multimap< const std::string, ProcInfo >  T_ProcList ;  
    //@}

    //! @name Copy/Assignment
    //@{
    ProcessList( const ProcessList& );            //!< Copy constructor
    ProcessList& operator=( const ProcessList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool Kill( ProcInfo& proc ) ; 
    //@}

private:
    
    
    //! @name Member data
    //@{
    T_ProcList processes_ ; 
    //@}
};

}

#endif // __ProcessList_h_
