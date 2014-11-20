// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __OptionWidgets_h_
#define __OptionWidgets_h_

#include "ColorButton.h"
#include "RichSpinBox.h"
#include "RichWidget.h"
#include "SignalAdapter.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/OptionVariant.h"

namespace gui
{
    template< typename T >
    class OptionWidget : public T
                       , public tools::Observer_ABC
                       , public kernel::OptionsObserver_ABC
    {
    public:
        OptionWidget( kernel::OptionsController& options,
                      const QString& objectName,
                      const std::string& optionName,
                      QWidget* parent )
            : T( objectName, parent )
            , options_( options )
            , optionName_( optionName )
        {
            options_.Register( *this );
        }
        ~OptionWidget()
        {
            options_.Unregister( *this );
        }

        virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value )
        {
            if( optionName_ == name )
                OnOptionChanged( value );
        }

    protected:
        virtual void OnOptionChanged( const kernel::OptionVariant& ) {}

    protected:
        kernel::OptionsController& options_;
        const std::string optionName_;
    };

    // -----------------------------------------------------------------------------

    class OptionColorButton : public OptionWidget< ColorButton >
    {
    public:
                OptionColorButton( kernel::OptionsController& options,
                                   const QString& objectName,
                                   const std::string& optionName,
                                   const QColor& color = Qt::black,
                                   QWidget* parent = 0 );
        virtual ~OptionColorButton();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionCheckBox : public OptionWidget< RichWidget< QCheckBox > >
    {
    public:
                OptionCheckBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& text,
                                QWidget* parent = 0 );
        virtual ~OptionCheckBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionSpinBox : public OptionWidget< RichSpinBox >
    {
    public:
                OptionSpinBox( kernel::OptionsController& options,
                               const QString& objectName,
                               const std::string& optionName,
                               int min,
                               int max,
                               QWidget* parent = 0 );
        virtual ~OptionSpinBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionSlider : public OptionWidget< RichWidget< QSlider > >
    {
    public:
                OptionSlider( kernel::OptionsController& options,
                              const QString& objectName,
                              const std::string& optionName,
                              int min,
                              int max,
                              QWidget* parent = 0 );
        virtual ~OptionSlider();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionGroupBox : public OptionWidget< RichWidget< QGroupBox > >
    {
    public:
                OptionGroupBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& title,
                                QWidget* parent = 0 );
        virtual ~OptionGroupBox();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    class OptionDial : public OptionWidget< RichWidget< QDial > >
    {
    public:
                OptionDial( kernel::OptionsController& options,
                            const QString& objectName,
                            const std::string& optionName,
                            int min,
                            int max,
                            QWidget* parent = 0 );
        virtual ~OptionDial();

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value );
    };

    // -----------------------------------------------------------------------------

    template< typename T >
    class OptionComboBox : public OptionWidget< RichWidget< QComboBox > >
    {
    public:
        OptionComboBox( kernel::OptionsController& options,
                        const QString& objectName,
                        const std::string& optionName,
                        QWidget* parent = 0 )
            : OptionWidget< RichWidget< QComboBox > >( options, objectName, optionName, parent )
        {
            setEditable( false );
            gui::connect( this, SIGNAL( currentIndexChanged( int ) ), [=,&options] {
                options.Change( optionName, itemData( currentIndex() ).value< T >() );
            } );
        }

        virtual ~OptionComboBox()
        {
            // NOTHING
        }

        void AddItem( const QString& text, const T& value )
        {
            blockSignals( true );
            addItem( text, value );
            blockSignals( false );
        }

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            int index = findData( value.To< T >() );
            if( index != -1 )
                setCurrentIndex( index );
        }
    };

} //! namespace gui

#endif // __OptionWidgets_h_
