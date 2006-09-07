// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Diplomacy_h_
#define __Diplomacy_h_

// =============================================================================
/** @class  Diplomacy
    @brief  Diplomacy
*/
// Created: SBO 2006-09-07
// =============================================================================
class Diplomacy
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacy();
    explicit Diplomacy( const QString& value, const QString& name );
             Diplomacy( const Diplomacy& );
    virtual ~Diplomacy();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetValue() const;
    static Diplomacy Unknown();
    static Diplomacy Friend ();
    static Diplomacy Enemy  ();
    static Diplomacy Neutral();

    Diplomacy& operator=( const Diplomacy& );
    //@}

private:
    //! @name Member data
    //@{
    QString value_;
    QString name_;
    //@}

private:
    //! @name Static members
    //@{
    static Diplomacy unknown_;
    static Diplomacy friend_;
    static Diplomacy enemy_;
    static Diplomacy neutral_;
    //@}
};

#endif // __Diplomacy_h_
