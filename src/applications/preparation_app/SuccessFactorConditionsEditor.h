// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorConditionsEditor_h_
#define __SuccessFactorConditionsEditor_h_

class ScoresModel;
class SuccessFactorConditions;
class SuccessFactorConditionItem;

namespace
{
    class ScrollView : public Q3ScrollView
    {
    public:
        explicit ScrollView( QWidget* parent )
            : Q3ScrollView( parent )
            , mainWidget_( new Q3VBox( viewport() ) )
        {
            setMargin( 2 );
            setHScrollBarMode( Q3ScrollView::AlwaysOff );
            setResizePolicy( Q3ScrollView::AutoOneFit );
            setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
            addChild( mainWidget_ );
        }
        virtual ~ScrollView() {}

        virtual void insertChild( QObject* object )
        {
            mainWidget_->insertChild( object );
        }

        virtual void removeChild( QObject* object )
        {
            mainWidget_->removeChild( object );
        }

        Q3VBox* getMainWidget() const
        {
            return mainWidget_;
        }
    private:
        Q3VBox* mainWidget_;
    };
}

// =============================================================================
/** @class  SuccessFactorConditionsEditor
    @brief  SuccessFactorConditionsEditor
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorConditionsEditor : public Q3VBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorConditionsEditor( QWidget* parent, const ScoresModel& scores );
    virtual ~SuccessFactorConditionsEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const SuccessFactorConditions& conditions );
    void CommitTo( SuccessFactorConditions& conditions ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    SuccessFactorConditionItem* CreateItem();
    void OnDelete( SuccessFactorConditionItem& item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorConditionsEditor( const SuccessFactorConditionsEditor& );            //!< Copy constructor
    SuccessFactorConditionsEditor& operator=( const SuccessFactorConditionsEditor& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< SuccessFactorConditionItem* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    const ScoresModel& scores_;
    Q3ButtonGroup* operator_;
    ScrollView* scrollView_;
    T_Items items_;
    //@}
};

#endif // __SuccessFactorConditionsEditor_h_
