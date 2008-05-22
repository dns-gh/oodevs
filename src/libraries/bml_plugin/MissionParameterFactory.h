// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterFactory_h_
#define __MissionParameterFactory_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class MissionType;
}

namespace bml
{
    class MissionParameter_ABC;

// =============================================================================
/** @class  MissionParameterFactory
    @brief  MissionParameterFactory
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterFactory();
    virtual ~MissionParameterFactory();
    //@}

    //! @name Operations
    //@{
    MissionParameter_ABC* CreateParameter( xml::xistream& xis, const kernel::MissionType& mission ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterFactory( const MissionParameterFactory& );            //!< Copy constructor
    MissionParameterFactory& operator=( const MissionParameterFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __MissionParameterFactory_h_
