using System;
using System.Timers;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MOLE;

namespace crossbow
{
    public interface IDynamicLayerDataset
    {
        IDataset Dataset { get; }
    }

    [Guid("e5798908-98b7-466f-9eee-43e58516f700")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.DynamicMoleLayer")]
    public class DynamicMoleLayer : BaseDynamicLayer, IFeatureLayer, IFeatureSelection, IDynamicLayerDataset
    {
        private class DynamicElement
        {
            public ICachedGraphic graphic;
            public IDynamicGlyph glyph;
            public float sizeRatio;
        };

        private IApplication                    m_application = null;
        private bool                            m_selectable = true;
        private IFeatureClass                   m_featureClass = null;
        private System.Collections.Hashtable    m_elements = new System.Collections.Hashtable();
        private SymbolFactory                   m_symbolFactory = new SymbolFactory();
        private FieldsProperty                  m_Fields = new FieldsProperty();
        private Timer                           m_updateTimer;
        private double                          m_symbolSize = 0;
        private IDynamicGlyph                   m_selectionGlyph = null;
        
        // IFeatureLayer attributes
        private String m_displayField = "name"; // $$$$ SBO 2007-07-09: Symbol_ID ?
        private bool m_scaleSymbols = false;

        // IFeatureSelection attributes
        private ISelectionSet m_selectionSet;
        private double m_bufferDistance = 0;
        private esriSelectionResultEnum m_combinationMethod = esriSelectionResultEnum.esriSelectionResultNew;
        private IColor m_selectionColor = null;
        private ISymbol m_selectionSymbol = null;
        private bool m_useSelectionSymbol = false;

        #region class constructor/destructor
        public DynamicMoleLayer() : base()
        {
            IRgbColor color = new RgbColorClass();
            color.Red = 255; color.Green = 0; color.Blue = 0;
            m_selectionColor = color;

            SetupTimer(100);
        }

        ~DynamicMoleLayer()
        {
            Disconnect();
        }

        public void Initialize(IApplication application)
        {
            m_application = application;
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

        #region Drawing feature
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

            IDynamicSymbolProperties properties     = dynamicDisplay as IDynamicSymbolProperties;
            IDynamicCompoundMarker   compoundMarker = dynamicDisplay as IDynamicCompoundMarker;

            IMxDocument mxDocument = Tools.GetMxDocument(m_application);
            IDisplayTransformation transformation = mxDocument.ActiveView.ScreenDisplay.DisplayTransformation;

            IPoint position = feature.Shape as IPoint;
            position.Project(transformation.SpatialReference);

            double zoom = symbol.sizeRatio * transformation.FromPoints(symbol.graphic.Size) / transformation.FittedBounds.Width;

            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, symbol.glyph);
            properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, (float)zoom, (float)zoom);
            if (IsSelected(feature))
            {
                {
                    IEnvelope envelope = new EnvelopeClass();
                    envelope = position.Envelope.Envelope;
                    float width = 0, height = 0;
                    symbol.glyph.QueryDimensions(ref width, ref height);
                    width = (float)(transformation.FromPoints(width) * zoom);
                    height = (float)(transformation.FromPoints(height) * zoom);
                    envelope.Expand(width / 2, height / 2, false);
                    if (m_selectionGlyph == null)
                    {
                        IDynamicGlyphFactory factory = dynamicDisplay as IDynamicGlyphFactory;
                        m_selectionGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);
                    }
                    properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_selectionGlyph);
                    properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
                    properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);
                    dynamicDisplay.DrawRectangle(envelope);
                }
                //properties.SetColor(esriDynamicSymbolType.esriDSymbolMarker, 0, 0, 0, 0.2f);
            }

            compoundMarker.DrawCompoundMarker4(position, "", "", (string)feature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eName]), "");
            properties.SetColor(esriDynamicSymbolType.esriDSymbolMarker, 1, 1, 1, 1);
        }

        private bool IsSelected(IFeature feature)
        {
            if (m_selectionSet == null)
                return false;
            IEnumIDs ids = m_selectionSet.IDs;
            ids.Reset();
            int id = ids.Next();
            while( id != -1 )
            {
                if (id == feature.OID)
                    return true;
                id = ids.Next();
            }
            return false;
        }

        private DynamicElement GetSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            String symbolId = Tools.GetFieldValueByName(feature, "Symbol_ID");
            DynamicElement element = (DynamicElement)m_elements[symbolId];
            if (element != null)
                return element;
            if (m_symbolFactory.SpatialReference == null)
                m_symbolFactory.SpatialReference = display.DisplayTransformation.SpatialReference;
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
            m_symbolSize = width;
            return element;
        }
        #endregion
        
        #region Timer methods
        void SetupTimer(double msec)
        {
            m_updateTimer = new Timer(msec);
            m_updateTimer.Enabled = true;
            m_updateTimer.Elapsed += new ElapsedEventHandler(OnLayerUpdateEvent);
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
                return Dataset.Category.ToString();
            }
            set
            {
                // $$$$ SBO 2007-07-09: Dataset.Category = value;
            }
        }

        public string DisplayField
        {
            get
            {
                return m_displayField;
            }
            set
            {
                m_displayField = value;
            }
        }

        public bool ScaleSymbols
        {
            get
            {
                return m_scaleSymbols;
            }
            set
            {
                m_scaleSymbols = value;
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
            set { }
        }

        #endregion

        #region IDataset Members
        public IDataset Dataset
        {
            get
            {
                return m_featureClass as IDataset;
            }
        }
        #endregion    
            
        #region IFeatureSelection Members

        public void Add(IFeature Feature)
        {
            m_selectionSet.Add(Feature.OID);
        }

        public double BufferDistance
        {
            get
            {
                return m_bufferDistance;
            }
            set
            {
                m_bufferDistance = value;
            }
        }

        public void Clear()
        {
            
        }

        public esriSelectionResultEnum CombinationMethod
        {
            get
            {
                return m_combinationMethod;
            }
            set
            {
                m_combinationMethod = value;
            }
        }

        public void SelectFeatures(IQueryFilter Filter, esriSelectionResultEnum Method, bool justOne)
        {
            if (Filter is ISpatialFilter)
            {
                ISpatialFilter spatialFilter = Filter as ISpatialFilter;

                IMxDocument mxDocument = Tools.GetMxDocument(m_application);
                double symbolWidth = mxDocument.ActiveView.ScreenDisplay.DisplayTransformation.FromPoints(m_symbolSize);
                double symbolHeight = symbolWidth;
                IEnvelope envelope = spatialFilter.Geometry.Envelope;
                symbolWidth  = (symbolWidth > envelope.Width) ? symbolWidth - envelope.Width : envelope.Width - symbolWidth;
                symbolHeight = (symbolHeight > envelope.Height) ? symbolHeight - envelope.Height : envelope.Height - symbolHeight;
                envelope.Expand(symbolWidth / 2, symbolHeight / 2, false);
                spatialFilter.Geometry = envelope;
            }

            esriSelectionOption option = justOne ? esriSelectionOption.esriSelectionOptionOnlyOne : esriSelectionOption.esriSelectionOptionNormal;
            ISelectionSet result = m_featureClass.Select(Filter, esriSelectionType.esriSelectionTypeHybrid, option, Dataset.Workspace);
            switch (Method)
            {
                case esriSelectionResultEnum.esriSelectionResultAdd:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetUnion, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultAnd:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetIntersection, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultNew:
                    m_selectionSet = result;
                    break;
                case esriSelectionResultEnum.esriSelectionResultSubtract:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetDifference, out m_selectionSet);
                    break;
                case esriSelectionResultEnum.esriSelectionResultXOR:
                    m_selectionSet.Combine(result, esriSetOperation.esriSetSymDifference, out m_selectionSet);
                    break;
                default:
                    break;
            }
        }

        public void SelectionChanged()
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        public IColor SelectionColor
        {
            get
            {
                return m_selectionColor;
            }
            set
            {
                m_selectionColor = value;
            }
        }

        public ISelectionSet SelectionSet
        {
            get
            {
                return m_selectionSet;
            }
            set
            {
                m_selectionSet = value;
            }
        }

        public ISymbol SelectionSymbol
        {
            get
            {
                return m_selectionSymbol;
            }
            set
            {
                m_selectionSymbol = value;
            }
        }

        public bool SetSelectionSymbol
        {
            get
            {
                return m_useSelectionSymbol;
            }
            set
            {
                m_useSelectionSymbol = value;
            }
        }

        #endregion
    }
}
