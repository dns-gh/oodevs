// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsContext_h_
#define __ActionsContext_h_

#include <string>
#include <map>

class Q3ListBox;
class QString;

// =============================================================================
/** @class  ActionsContext
    @brief  ActionsContext
*/
// Created: SBO 2008-04-16
// =============================================================================
class ActionsContext
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsContext();
    virtual ~ActionsContext();
    //@}

    //! @name Widget operations
    //@{
    void Load( const std::string& name, Q3ListBox* widget ) const;
    void Save( const std::string& name, const Q3ListBox* widget );
    void Save( const std::string& name, const QString& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsContext( const ActionsContext& );            //!< Copy constructor
    ActionsContext& operator=( const ActionsContext& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::string GetValue( const std::string& name ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    T_Values values_;
    //@}
};

#endif // __ActionsContext_h_
