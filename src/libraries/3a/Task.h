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
#include <map>
#include <boost/enable_shared_from_this.hpp>

class Connector_ABC;
class ModelFunction_ABC;
class Slot_ABC;
class Result_ABC;

namespace dispatcher
{
    class MessageLoader_ABC;
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
           , public boost::enable_shared_from_this< Task >
{
public:
    //! @name Constructors/Destructor
    //@{
             Task( unsigned int firstTick, unsigned int lastTick );
    virtual ~Task();
    //@}

    //! @name Operations
    //@{
    void SetResult( boost::shared_ptr< Result_ABC > output );
    void AddExtractor( boost::shared_ptr< ModelFunction_ABC > function );
    void AddFunction( const std::string& name, boost::shared_ptr< Slot_ABC > function );
    void AddFunction( boost::shared_ptr< Slot_ABC > function );
    void AddConnector( const std::string& name, boost::shared_ptr< Connector_ABC > connector );
    void Connect( xml::xistream& xis );

    virtual void Receive( const sword::SimToClient& message );

    void Commit();
    void Process( dispatcher::MessageLoader_ABC& loader );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< Connector_ABC > > T_Connectors;
    typedef T_Connectors::const_iterator                              CIT_Connectors;

    typedef std::map< std::string, boost::shared_ptr< Slot_ABC > > T_Slots;
    typedef T_Slots::const_iterator                              CIT_Slots;

    typedef std::vector< boost::shared_ptr< Slot_ABC > > T_Objects;
    //@}

    //! @name Helpers
    //@{
    void Connect( const std::string& , xml::xistream& xis );
    void Connect( const std::string& name, const std::string& input );
    void Connect( const std::string& name, const std::string& input, unsigned int i );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int firstTick_;
    unsigned int lastTick_;
    unsigned int firstTickRead_;
    bool hasFirstTick_;
    unsigned int currentTick_;
    bool insideFrame_;
    ModelFunctionComposite composite_;
    T_Connectors connectors_;
    T_Slots slots_;
    T_Objects objects_;
    boost::shared_ptr< Result_ABC > result_;
    //@}
};

#endif // __Task_h_
