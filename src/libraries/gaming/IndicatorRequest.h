// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndicatorRequest_h_
#define __IndicatorRequest_h_

#include "game_asn/Aar.h"

namespace kernel
{
    class Controller;
}

class IndicatorDefinition_ABC;
class Publisher_ABC;

// =============================================================================
/** @class  IndicatorRequest
    @brief  IndicatorRequest
*/
// Created: AGE 2007-09-25
// =============================================================================
class IndicatorRequest
{
public:
    //! @name Types
    //@{
    typedef std::vector< double > T_Data;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorRequest( kernel::Controller& controller, const IndicatorDefinition_ABC& definition, Publisher_ABC& publisher );
    virtual ~IndicatorRequest();
    //@}

    //! @name Operations
    //@{
    void SetParameter( const std::string& name, const std::string& value );
    void Commit() const;

    QString GetName() const;
    void Update( const ASN1T_MsgPlotResult& message );
    void Update( const ASN1T_MsgIndicator& message );
    bool IsPending() const;
    bool IsDone() const;
    bool IsFailed() const;

    QString ErrorMessage() const;
    const T_Data& Result() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorRequest( const IndicatorRequest& );            //!< Copy constructor
    IndicatorRequest& operator=( const IndicatorRequest& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const IndicatorDefinition_ABC& definition_;
    Publisher_ABC& publisher_;
    T_Parameters parameters_;
    bool done_;
    T_Data result_;
    T_Data newValues_;
    std::string error_;
    //@}
};

#endif // __IndicatorRequest_h_
