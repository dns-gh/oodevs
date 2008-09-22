// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionRequest_h_
#define __AfterActionRequest_h_

#include "game_asn/Aar.h"

namespace kernel
{
    class Controller;
}

class AfterActionFunction;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionRequest
    @brief  AfterActionRequest
*/
// Created: AGE 2007-09-25
// =============================================================================
class AfterActionRequest
{
public:
    //! @name Types
    //@{
    typedef std::vector< double > T_Data;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequest( kernel::Controller& controller, const AfterActionFunction& function, Publisher_ABC& publisher );
    virtual ~AfterActionRequest();
    //@}

    //! @name Operations
    //@{
    void SetParameter( const std::string& name, const std::string& value );
    void Commit();

    QString GetName() const;
    void Update( const ASN1T_MsgPlotResult& asnMsg );
    bool IsPending() const;
    bool IsDone() const;
    bool IsFailed() const;

    QString ErrorMessage() const;
    const T_Data& Result() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionRequest( const AfterActionRequest& );            //!< Copy constructor
    AfterActionRequest& operator=( const AfterActionRequest& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const AfterActionFunction& function_;
    Publisher_ABC& publisher_;
    T_Parameters parameters_;
    bool done_;
    T_Data result_;
    std::string error_;
    //@}
};

#endif // __AfterActionRequest_h_
