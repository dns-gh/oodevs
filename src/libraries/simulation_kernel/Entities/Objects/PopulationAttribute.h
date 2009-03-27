// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationAttribute_h_
#define __PopulationAttribute_h_

#include "ObjectAttribute_ABC.h"

class MIL_AutomateLOG;

class PopulationAttribute : public ObjectAttribute_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	        PopulationAttribute();
	virtual ~PopulationAttribute();
	//@}

	//! @name Operations
    //@{
    MT_Float GetDensity() const;
    void  Reset();
    void  SetDensity( MT_Float rDensity );    
	//@}

    //! @name Copy
    //@{
    PopulationAttribute& operator=( const PopulationAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    PopulationAttribute( const PopulationAttribute& from );
    //@}

private:
    //! @name 
    //@{
//    ASN1T_ObjectAttributesLogistic asnAttributes_;
    MT_Float    density_;
    //@}
};

#endif //__PopulationAttribute_h_