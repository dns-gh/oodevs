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

namespace dispatcher
{
    class Automat;
}

namespace bml
{
    class MissionParameter_ABC;
    class Mission;

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
             MissionParameterFactory( const Mission& mission, const kernel::MissionType& type, const dispatcher::Automat& automat );
    virtual ~MissionParameterFactory();
    //@}

    //! @name Operations
    //@{
    MissionParameter_ABC* CreateParameter( xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterFactory( const MissionParameterFactory& );            //!< Copy constructor
    MissionParameterFactory& operator=( const MissionParameterFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Mission& mission_;
    const kernel::MissionType& type_;
    const dispatcher::Automat& automat_;
    //@}
};

}

#endif // __MissionParameterFactory_h_
