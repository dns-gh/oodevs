// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Score_h_
#define __Score_h_

#include "IndicatorDefinition_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "protocol/AarSenders.h"

using namespace Common;

namespace kernel
{
    class Controller;
}

namespace indicators
{
    class Gauge;
    class Tendency;
}

class IndicatorRequest;
class Publisher_ABC;
class ScoreDefinition;
class ScoreDefinitions;

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-03-12
// =============================================================================
class Score : public IndicatorDefinition_ABC
            , public kernel::Displayable_ABC

{
public:
    //! @name Constructors/Destructor
    //@{
             Score( const MsgsAarToClient::MsgIndicator& message, const ScoreDefinitions& definitions, kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual std::string Commit( const T_Parameters& parameters ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    void Update( const MsgsAarToClient::MsgIndicator& message );
    void ConnectTo( IndicatorRequest& request );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score( const Score& );            //!< Copy constructor
    Score& operator=( const Score& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void UpdatePlots( const MsgsAarToClient::MsgIndicator& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    const QString name_;
    const ScoreDefinition& definition_;
    std::auto_ptr< indicators::Tendency > tendency_;
    std::auto_ptr< indicators::Gauge > gauge_;
    double value_;
    IndicatorRequest* request_;
    //@}
};

#endif // __Score_h_
