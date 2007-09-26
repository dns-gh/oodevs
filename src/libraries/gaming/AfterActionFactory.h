// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionFactory_h_
#define __AfterActionFactory_h_

#include "AfterActionItem.h"

namespace xml
{
    class xistream;
};
class AfterActionFunction;

// =============================================================================
/** @class  AfterActionFactory
    @brief  AfterActionFactory
*/
// Created: AGE 2007-09-19
// =============================================================================
class AfterActionFactory : public AfterActionItem
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFactory( const std::string& type, xml::xistream& xis );
    virtual ~AfterActionFactory();
    //@}

    //! @name Operations
    //@{
    const QString& GetName() const;
    const QString& GetType() const;
    AfterActionItem_ABC* Create( AfterActionFunction& function ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionFactory( const AfterActionFactory& );            //!< Copy constructor
    AfterActionFactory& operator=( const AfterActionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QString type_;
    QString name_;
    //@}
};

#endif // __AfterActionFactory_h_
