// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __edxl_EDXLHaveApp_h_
#define __edxl_EDXLHaveApp_h_

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace xml
{
    class xistream;
}

namespace sword
{
    class SimToClient;
}

class SwordClient;

namespace edxl
{
    class PublisherActor;
    class ReportManager;
    class Simulation;
    class UpdateListener;
    class ReportBuilder_ABC;
    class Model;

// =============================================================================
/** @class  EDXLHaveApp
    @brief  EDXLHaveApp
*/
// Created: JCR 2010-05-29
// =============================================================================
class EDXLHaveApp
{

public:
    //! @name Constructors/Destructor
    //@{
             EDXLHaveApp( const boost::program_options::variables_map& options );
    virtual ~EDXLHaveApp();
    //@}

    //! @name Operations
    //@{
    void Run();
    //@}

private:
    //! @name Helpers
    //@{
    void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    boost::program_options::variables_map options_;
    std::auto_ptr< Model > model_;
    std::auto_ptr< SwordClient > client_;
    std::auto_ptr< ReportBuilder_ABC > reports_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< PublisherActor > publisher_;
    std::auto_ptr< UpdateListener > listener_;
    //@}
};

}

#endif // __edxl_EDXLHaveApp_h_
