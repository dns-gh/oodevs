// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_h_
#define __MissionInterface_h_

#include "clients_kernel/Resolver.h"
#include "game_asn/Asn.h"

namespace xml
{
    class xistream;
}

class MissionParameter;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionTemplate
    @brief  Mission template
*/
// Created: SBO 2006-11-22
// =============================================================================
class MissionTemplate
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionTemplate( xml::xistream& xis );
    virtual ~MissionTemplate();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    void BuildInterface( MissionInterfaceBuilder& builder, ASN1T_MissionParameters& mission ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionTemplate( const MissionTemplate& );            //!< Copy constructor
    MissionTemplate& operator=( const MissionTemplate& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< MissionParameter* >  T_Parameters;
    typedef T_Parameters::const_iterator    CIT_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    QString name_;
    T_Parameters parameters_;
    //@}
};

#endif // __MissionInterface_h_
