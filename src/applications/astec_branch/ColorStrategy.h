// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorStrategy_h_
#define __ColorStrategy_h_

#include "ColorStrategy_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "TeamSelectionObserver.h"

class Team;
class Controllers;

// =============================================================================
/** @class  ColorStrategy
    @brief  ColorStrategy
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy : public ColorStrategy_ABC
                    , private Observer_ABC
                    , public ElementObserver_ABC< Team >
                    , public TeamSelectionObserver
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorStrategy( Controllers& controllers );
    virtual ~ColorStrategy();
    //@}

    //! @name Operations
    //@{
    virtual void SelectColor( const Agent& agent );
    virtual void SelectColor( const Object& object );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorStrategy( const ColorStrategy& );            //!< Copy constructor
    ColorStrategy& operator=( const ColorStrategy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team& );
    virtual void NotifyUpdated( const Team& );
    virtual void NotifyDeleted( const Team& );

    virtual void BeforeSelection();
    virtual void Select( const Agent& element );
    virtual void Select( const Object& element );
    virtual void Select( const Team* );

    void InitializeSynonyms();
    void InitializeColors();

    std::string RandomName() const;
    QColor RandomColor() const;
    void CreateNewColor( const std::string& name );
    //@}

    //! @name Types 
    //@{
    typedef std::map< const Team*, std::pair< std::string, QColor > > T_TeamColors;
    typedef std::map< std::string, std::string >                      T_Synonyms;
    typedef std::map< std::string, QColor >                           T_Colors;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Team*   selectedTeam_;
    const Object* selectedObject_;
    const Agent*  selectedAgent_;

    T_TeamColors teamColors_;
    T_Synonyms   synonyms_;
    T_Colors     available_;
    //@}
};

#endif // __ColorStrategy_h_
