// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_h_
#define __MissionParameter_h_

#include "gaming/ASN_Types.h"

namespace xml
{
    class xistream;
}

class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionParameter
    @brief  Mission parameter
*/
// Created: SBO 2006-11-22
// =============================================================================
class MissionParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter( xml::xistream& xis );
    virtual ~MissionParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    void BuildInterface( MissionInterfaceBuilder& builder, ASN1T_MissionParameter& asnParam ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionParameter( const MissionParameter& );            //!< Copy constructor
    MissionParameter& operator=( const MissionParameter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadValue( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< int, std::string > > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString diaName_;
    QString type_;
    bool optional_;
    T_Values values_;
    //@}
};

#endif // __MissionParameter_h_
