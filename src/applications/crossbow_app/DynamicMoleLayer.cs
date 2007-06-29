using System;
using System.Timers;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.Carto;

namespace crossbow
{
    public interface IDynamicLayerDataset
    {        
        IDataset Dataset { get; }
    }

    [Guid("e5798908-98b7-466f-9eee-43e58516f700")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.DynamicMoleLayer")]
    public class DynamicMoleLayer : BaseDynamicLayer, IFeatureLayer, IDynamicLayerDataset, IFeatureSelection
    {
        private class DynamicElement
        {
            public ICachedGraphic graphic;
            public IDynamicGlyph glyph;
            public float sizeRatio;
        };

        private bool                            m_selectable;
        private IFeatureClass                   m_featureClass;
        private System.Collections.Hashtable    m_elements;
        private SymbolFactory                   m_symbolFactory;
        private FieldsProperty                  m_Fields;
        private Timer                           m_updateTimer;
        // private FeatureSelection

        #region class constructor/destructor        
        public DynamicMoleLayer() : base()
        {
            m_selectable = false;
            m_featureClass = null;
            m_symbolFactory = new SymbolFactory();
            m_elements = new System.Collections.Hashtable();
            m_Fields = new FieldsProperty();
            SetupTimer(100);
        }

        ~DynamicMoleLayer()
        {            
            m_symbolFactory = null;
            m_elements = null;
            m_Fields = null;
        }
        #endregion

        #region BaseDynamicLayer overriden
        public override void DrawDynamicLayer(esriDynamicDrawPhase dynamicDrawPhase, IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            try
            {
                //make sure that the current drawphase if immediate. In this sample there is no use of the
                //compiled drawPhase. Use the esriDDPCompiled drawPhase in order to draw semi-static items (items
                //whose update rate is lower than the display update rate).
                if( dynamicDisplay == null || display == null || dynamicDrawPhase != esriDynamicDrawPhase.esriDDPImmediate )
                    return;
                
                if( this.m_visible )
                    DrawFeatureClass(display, dynamicDisplay);
            }
            catch( Exception ex )
            {
                System.Diagnostics.Trace.WriteLine(ex);
            }
        }

        #endregion

        #region Drawing featue
        private void DrawFeatureClass(IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            if (m_featureClass == null)
                return;
            IFeatureCursor cursor = m_featureClass.Search(null, false);
            IFeature feature = cursor.NextFeature();
            while (feature != null)
            {
                DrawFeature(display, dynamicDisplay, feature);
                feature = cursor.NextFeature();
            }
        }

        private void DrawFeature(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {            
            DynamicElement symbol = GetSymbol(display, dynamicDisplay, feature);
            if (symbol == null)
                return;

            IDynamicSymbolProperties    properties = (IDynamicSymbolProperties)dynamicDisplay;
            IDynamicCompoundMarker      compoundMarker = (IDynamicCompoundMarker)dynamicDisplay;

            IPoint position = (IPoint)feature.Shape;
            position.Project(display.DisplayTransformation.SpatialReference);

            double zoom = symbol.sizeRatio * display.DisplayTransformation.FromPoints(symbol.graphic.Size) / display.DisplayTransformation.FittedBounds.Width;
            
            //set the symbol and text alignment so that it will align with the screen            
            properties.set_RotationAlignment(esriDynamicSymbolType.esriDSymbolMarker, esriDynamicSymbolRotationAlignment.esriDSRAScreen);
            properties.set_RotationAlignment(esriDynamicSymbolType.esriDSymbolText, esriDynamicSymbolRotationAlignment.esriDSRAScreen);
            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, symbol.glyph);
            properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, (float)zoom, (float)zoom);
            properties.set_Smooth(esriDynamicSymbolType.esriDSymbolLine, true);
            properties.set_Smooth(esriDynamicSymbolType.esriDSymbolMarker, true);
            compoundMarker.DrawCompoundMarker4( position, "", "", (string)feature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eName]), "");
            // dynamicDisplay.DrawMarker(position);
        }

        private DynamicElement GetSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            String symbolId = Tools.GetFieldValueByName(feature, "Symbol_ID");
            DynamicElement element = (DynamicElement)m_elements[symbolId];
            if (element != null)
                return element;
            if ( m_symbolFactory.SpacialReference == null )
                m_symbolFactory.SpacialReference = display.DisplayTransformation.SpatialReference;
            element = BuildSymbol(display, dynamicDisplay, symbolId); // throw something
            m_elements[symbolId] = element;
            return element;
        }

        private DynamicElement BuildSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, String symbolId)
        {
            const int size = 64;            
            DynamicElement element = new DynamicElement();            
            element.graphic = m_symbolFactory.CreateGraphics(symbolId, "");
            element.glyph = m_symbolFactory.CreateDynamicGlyph(display, dynamicDisplay, element.graphic, size);

            float width = 0, height = 0;
            element.glyph.QueryDimensions(ref width, ref height);
            element.sizeRatio = (float)size / width;
            // element.sizeRatio = 1;
            return element;
        }
        #endregion


        #region Timer methods
        void SetupTimer(double msec)
        {
            this.m_updateTimer = new Timer(msec);
            this.m_updateTimer.Enabled = false;
            this.m_updateTimer.Elapsed += new ElapsedEventHandler(OnLayerUpdateEvent);
        }
        
        public void Connect()
        {
            m_updateTimer.Enabled = true;
        }

        public void Disconnect()
        {
            m_updateTimer.Enabled = false;
        }

        /// <summary>
        /// timer elapsed event handler, used to update the layer
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <remarks>This layer has synthetic data, and therefore need the timer in order
        /// to update the layers' items.</remarks>
        private void OnLayerUpdateEvent(object sender, ElapsedEventArgs e)
        {
            try
            {
                // Update();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.Message);
            }
        }
        #endregion

        #region IFeatureLayer Members

        public IFeatureClass FeatureClass
        {
            get
            {
                return m_featureClass;
            }
            set
            {
                m_featureClass = value;
                m_selectable = true;
                m_Fields.SetupTacticalElementFields(m_featureClass);
            }
        }

        public string DataSourceType
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public string DisplayField
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public bool ScaleSymbols
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public IFeatureCursor Search(IQueryFilter queryFilter, bool recycling)
        {
            return m_featureClass.Search(queryFilter, recycling);
        }

        public bool Selectable
        {
            get
            {
                return m_selectable;
            }
            set
            {
                m_selectable = value;
            }
        }

        public new ISpatialReference SpatialReference
        {
            set { throw new Exception("The method or operation is not implemented."); }
        }

        #endregion

        #region IDataset Members        
        public IDataset Dataset
        {
            get
            {
                return (IDataset)m_featureClass;
            }
        }
        #endregion    
    
        #region IFeatureSelection Members

        public void Add(IFeature Feature)
        {
            throw new Exception("The method or operation is not implemented.");
        }

        public double BufferDistance
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public void Clear()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        public esriSelectionResultEnum CombinationMethod
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public void SelectFeatures(IQueryFilter Filter, esriSelectionResultEnum Method, bool justOne)
        {
            
            throw new Exception("The method or operation is not implemented.");
        }

        public void SelectionChanged()
        {
            throw new Exception("The method or operation is not implemented.");
        }

        public IColor SelectionColor
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public ISelectionSet SelectionSet
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public ISymbol SelectionSymbol
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public bool SetSelectionSymbol
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        #endregion
    }
}
