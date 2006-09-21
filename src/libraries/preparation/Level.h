// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Level_h_
#define __Level_h_

// =============================================================================
/** @class  Level
    @brief  Level
*/
// Created: SBO 2006-09-21
// =============================================================================
class Level
{

public:
    //! @name Constructors/Destructor
    //@{
             Level( const QString& name, const Level* previous );
    virtual ~Level();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    unsigned int GetId() const;
    const Level* GetPrevious() const;
    const Level* GetNext() const;
    //@}

    //! @name Operations
    //@{
    void SetNext( const Level& next );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Level( const Level& );            //!< Copy constructor
    Level& operator=( const Level& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    unsigned int id_;
    const Level* previous_;
    const Level* next_;
    //@}

private:
    //! @name Static
    //@{
    static unsigned int idManager_;
    //@}
};

#endif // __Level_h_
