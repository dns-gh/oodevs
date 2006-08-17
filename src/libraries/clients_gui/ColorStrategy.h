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
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"

class Team_ABC;
class Agent_ABC;
class Object_ABC;
class Population_ABC;
class Controllers;
class TacticalLine_ABC;
class Lima;
class Limit;
class GlTools_ABC;

// =============================================================================
/** @class  ColorStrategy
    @brief  ColorStrategy
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy : public ColorStrategy_ABC
                    , public Observer_ABC
                    , public ElementObserver_ABC< Team_ABC >
                    , public SelectionObserver_ABC
                    , public SelectionObserver_Base< Agent_ABC >
                    , public SelectionObserver_Base< Object_ABC >
                    , public SelectionObserver_Base< Population_ABC >
                    , public SelectionObserver_Base< Lima >
                    , public SelectionObserver_Base< Limit >
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy( Controllers& controllers, GlTools_ABC& tools );
    virtual ~ColorStrategy();
    //@}

    //! @name Operations
    //@{
    virtual void SelectColor( const Agent_ABC& agent );
    virtual void SelectColor( const Object_ABC& object );
    virtual void SelectColor( const Population_ABC& population );
    virtual void SelectColor( const AgentKnowledge& k );
    virtual void SelectColor( const ObjectKnowledge& k );
    virtual void SelectColor( const PopulationKnowledge& k );
    virtual void SelectColor( const TacticalLine_ABC& line );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorStrategy( const ColorStrategy& );            //!< Copy constructor
    ColorStrategy& operator=( const ColorStrategy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team_ABC& );
    virtual void NotifyDeleted( const Team_ABC& );

    virtual void BeforeSelection();
    virtual void Select( const Agent_ABC& element );
    virtual void Select( const Object_ABC& element );
    virtual void Select( const Population_ABC& element );
    virtual void Select( const Lima& element );
    virtual void Select( const Limit& element );
    virtual void AfterSelection();

    void InitializeSynonyms();
    void InitializeColors();

    std::string RandomName() const;
    QColor RandomColor() const;
    void CreateNewColor( const std::string& name );
    QColor SelectedColor( const QColor& base ) const;
    QColor SuperiorSelectedColor( const QColor& base ) const;
    QColor KnowledgeColor( const QColor& base ) const;
    void ApplyColor( const QColor& color ) const;
    //@}

    //! @name Types 
    //@{
    typedef std::map< const Team_ABC*, std::pair< std::string, QColor > > T_TeamColors;
    typedef std::map< std::string, std::string >                      T_Synonyms;
    typedef std::map< std::string, QColor >                           T_Colors;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    GlTools_ABC& tools_;

    const Object_ABC*     selectedObject_;
    const Agent_ABC*      selectedAgent_;
    const Agent_ABC*      selectedSuperior_;
    const Population_ABC* selectedPopulation_;
    const TacticalLine_ABC* selectedLine_;

    T_TeamColors teamColors_;
    T_Synonyms   synonyms_;
    T_Colors     available_;
    //@}
};

#endif // __ColorStrategy_h_
