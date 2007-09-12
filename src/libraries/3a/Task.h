// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Task_h_
#define __Task_h_

#include "ModelFunctionComposite.h"
#include "dispatcher/MessageHandler_ABC.h"
#include <memory>
#include <string>
#include <map>

class Connector_ABC;
class ModelFunction_ABC;
class Slot_ABC;
class Result_ABC;

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Task
    @brief  Task
*/
// Created: AGE 2007-09-11
// =============================================================================
class Task : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Task( dispatcher::ClientPublisher_ABC& publisher );
    virtual ~Task();
    //@}

    //! @name Operations
    //@{
    void SetResult   ( std::auto_ptr< Result_ABC > output );
    void AddExtractor( std::auto_ptr< ModelFunction_ABC > function );
    void AddFunction ( const std::string& name, std::auto_ptr< Slot_ABC > function );
    void AddConnector( const std::string& name, std::auto_ptr< Connector_ABC > connector );
    void Connect( xml::xistream& xis );

    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    void Commit(); // $$$$ AGE 2007-09-12: temp
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Task( const Task& );            //!< Copy constructor
    Task& operator=( const Task& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, Connector_ABC* >   T_Connectors;
    typedef T_Connectors::const_iterator            CIT_Connectors;
    typedef std::map< std::string, Slot_ABC* >        T_Slots;
    typedef T_Slots::const_iterator                 CIT_Slots;
    //@}

    //! @name Helpers
    //@{
    void Connect( const std::string& , xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    ModelFunctionComposite composite_;
    T_Connectors connectors_;
    T_Slots slots_;
    Result_ABC* result_;
    //@}
};

#endif // __Task_h_
