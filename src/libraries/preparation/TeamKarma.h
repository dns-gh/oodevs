// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamKarma_h_
#define __TeamKarma_h_

// =============================================================================
/** @class  TeamKarma
    @brief  TeamKarma
*/
// Created: SBO 2006-10-27
// =============================================================================
class TeamKarma
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamKarma( const QString& name, const QString& value, char id );
    virtual ~TeamKarma();
    //@}

    //! @name Operations
    //@{
    const QString& GetName() const;
    const QString& GetValue() const;
    char GetIdentifier() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamKarma( const TeamKarma& );            //!< Copy constructor
    TeamKarma& operator=( const TeamKarma& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString value_;
    char id_;
    //@}
};

#endif // __TeamKarma_h_
