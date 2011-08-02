// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StringQVButtonGroup_h_
#define __StringQVButtonGroup_h_

#include <vector>

// =============================================================================
/** @class  StringQVButtonGroup
    @brief  StringQVButtonGroup
*/
// Created: HBD 2010-09-16
// =============================================================================
class StringQVButtonGroup : public Q3VButtonGroup
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             StringQVButtonGroup( const QString& title, QWidget* parent );
    virtual ~StringQVButtonGroup();
    //@}

    //! @name Operations
    //@{
    void InsertCheckbox( const std::string& name );
   //@}

signals:
    //! @name signals
    //@{
    void clicked( const std::string& );
    //@}

public slots:
    //! @name slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StringQVButtonGroup( const StringQVButtonGroup& );            //!< Copy constructor
    StringQVButtonGroup& operator=( const StringQVButtonGroup& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > values_;
    //@}
};

#endif // __StringQVButtonGroup_h_
