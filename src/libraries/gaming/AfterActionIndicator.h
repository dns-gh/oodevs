// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionIndicator_h_
#define __AfterActionIndicator_h_

#include <string>
#include "game_asn/Aar.h"

namespace kernel
{
	class Controller;
}

class AfterActionIndicator
{
public:
    //! @name Constructor / Destructor
    //@{
	AfterActionIndicator( const std::string& name, kernel::Controller& controller );
	~AfterActionIndicator();
    //@}

    //! @name Accessors
    //@{
	const QString&  GetName() const;
	double          GetValue() const;
    //@}

    //! @name Update
    //@{
	void Update( const ASN1T_MsgIndicator& asn );
    //@}

private:
    //! @name Copy/Assigment
    //@{
	AfterActionIndicator( const AfterActionIndicator& );
	const AfterActionIndicator& operator=( const AfterActionIndicator& );
    //@}

private:
	//! @name Member data
    //@{
    kernel::Controller& controller_;
	QString name_;
	double value_;
	// @}
};

#endif // __AfterActionIndicator_h_