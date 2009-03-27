// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionModel_h_
#define __AfterActionModel_h_

#include "game_asn/Aar.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
}
namespace xml
{
    class xistream;
}

class AfterActionFunction;
class AfterActionRequest;
class AfterActionRequests;
class AfterActionIndicator;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionModel
    @brief  AfterActionModel
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionModel : public kernel::Resolver< AfterActionFunction, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~AfterActionModel();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgAarInformation& asnMsg );
    void Update( const ASN1T_MsgPlotResult& asnMsg );
	void Update( const ASN1T_MsgIndicator& asnMsg );

    AfterActionRequest& CreateRequest( const AfterActionFunction& function );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionModel( const AfterActionModel& );            //!< Copy constructor
    AfterActionModel& operator=( const AfterActionModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load( const std::string& functions );
    void ReadFunction( xml::xistream& xis );
	void SaveIndicators( const std::string& fileName ) const;
    void WriteIndicators( xml::xostream& xos ) const;
    void WriteFile( const std::string& fileName, const std::string& data ) const;
    //@}	

private:
    //! @name Types
    //@{
	typedef std::map< std::string, AfterActionIndicator* > T_Indicators;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::auto_ptr< AfterActionRequests > requests_;
	T_Indicators indicators_;
    //@}
};

#endif // __AfterActionModel_h_
