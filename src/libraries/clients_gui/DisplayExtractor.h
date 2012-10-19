// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DisplayExtractor_h_
#define __DisplayExtractor_h_

#include "clients_kernel/DisplayExtractor_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Team_ABC;
    class Population_ABC;
    class Inhabitant_ABC;
    class KnowledgeGroup_ABC;
    class UrbanObject_ABC;
    class AgentKnowledge_ABC;
    class PopulationKnowledge_ABC;
    class ObjectKnowledge_ABC;
}

namespace gui
{

// =============================================================================
/** @class  DisplayExtractor
    @brief  DisplayExtractor
*/
// Created: JSR 2012-10-18
// =============================================================================
class DisplayExtractor : public QObject
                       , public kernel::DisplayExtractor_ABC
                       , public kernel::DisplayExtractor< QString >
                       , public kernel::DisplayExtractor< std::string >
                       , public kernel::DisplayExtractor< int >
                       , public kernel::DisplayExtractor< unsigned int >
                       , public kernel::DisplayExtractor< float >
                       , public kernel::DisplayExtractor< double >
                       , public kernel::DisplayExtractor< long >
                       , public kernel::DisplayExtractor< unsigned long >
                       , public kernel::DisplayExtractor< bool >
                       , public kernel::DisplayExtractor< kernel::Entity_ABC >
                       , public kernel::DisplayExtractor< kernel::Agent_ABC >
                       , public kernel::DisplayExtractor< kernel::Automat_ABC >
                       , public kernel::DisplayExtractor< kernel::Formation_ABC >
                       , public kernel::DisplayExtractor< kernel::Team_ABC >
                       , public kernel::DisplayExtractor< kernel::Population_ABC>
                       , public kernel::DisplayExtractor< kernel::Inhabitant_ABC>
                       , public kernel::DisplayExtractor< kernel::KnowledgeGroup_ABC >
                       , public kernel::DisplayExtractor< kernel::UrbanObject_ABC >
                       , public kernel::DisplayExtractor< kernel::AgentKnowledge_ABC >
                       , public kernel::DisplayExtractor< kernel::PopulationKnowledge_ABC >
                       , public kernel::DisplayExtractor< kernel::ObjectKnowledge_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             DisplayExtractor( QObject* parent );
    virtual ~DisplayExtractor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyLinkClicked( const QString& url );

    virtual QString GetDisplayName( const QString& element ) const;
    virtual QString GetDisplayName( const std::string& element ) const;
    virtual QString GetDisplayName( const int& element ) const;
    virtual QString GetDisplayName( const unsigned int& element ) const;
    virtual QString GetDisplayName( const float& element ) const;
    virtual QString GetDisplayName( const double& element ) const;
    virtual QString GetDisplayName( const long& element ) const;
    virtual QString GetDisplayName( const unsigned long& element ) const;
    virtual QString GetDisplayName( const bool& element ) const;

    virtual QString GetDisplayName( const kernel::Entity_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Agent_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Automat_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Formation_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Team_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Population_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::Inhabitant_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::KnowledgeGroup_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::UrbanObject_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::AgentKnowledge_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::PopulationKnowledge_ABC& element ) const;
    virtual QString GetDisplayName( const kernel::ObjectKnowledge_ABC& element ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void LinkClicked( const QString& link );
    //@}

private:
    //! @name Member data
    //@{
    static QLocale locale_;
    //@}
};

} // namespace gui

#endif // __DisplayExtractor_h_
