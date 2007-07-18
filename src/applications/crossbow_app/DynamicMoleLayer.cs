using System;
using System.Timers;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.esriSystem;
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
            public float aspectRatio; // glyph.width / glyph.height
        };

        private bool                            m_selectable = true;
        private IFeatureClass                   m_featureClass = null;
        private int                             m_featureClassId = -1;
        private System.Collections.Hashtable    m_elements = new System.Collections.Hashtable();
        private SymbolFactory                   m_symbolFactory = new SymbolFactory();
        private FieldsProperty                  m_Fields = new FieldsProperty();
        private Timer                           m_updateTimer;
        private double                          m_symbolSize = 0;
        private IDynamicGlyph                   m_selectionGlyph = null;
        private IEnvelope                       m_selectionEnvelope = null;
        
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
        #endregion

        #region BaseDynamicLayer overriden
        void ILayer.Draw(esriDrawPhase DrawPhase, IDisplay Display, ITrackCancel TrackCancel)
        {
            Tools.EnableDynamicDisplay();
        }

        public override void DrawDynamicLayer(esriDynamicDrawPhase dynamicDrawPhase, IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            try
            {
                if( dynamicDisplay == null || display == null || dynamicDrawPhase != esriDynamicDrawPhase.esriDDPImmediate )
                    return;
                
                if( this.m_visible )
                    DrawFeatureClass(display, dynamicDisplay);
                base.m_bIsImmediateDirty = true;
            }
            catch( Exception ex )
            {
                System.Diagnostics.Trace.WriteLine(ex);
            }
        }
        #endregion

        #region IPersistVariant overriden
        public override UID ID
        {
            get
            {
                UID id = new UIDClass();
                id.Value = "crossbow.DynamicMoleLayer";
                return id;
            }
        }

        public override void Load(IVariantStream Stream)
        {
            base.Load(Stream);
            m_selectable = (bool)Stream.Read();
            m_featureClassId = (int)Stream.Read(); m_featureClass = null;
            //m_elements = Stream.Read() as System.Collections.Hashtable;
            //m_symbolFactory = Stream.Read() as SymbolFactory;
            //m_Fields = Stream.Read() as FieldsProperty;
            //m_updateTimer = Stream.Read() as Timer;
            m_symbolSize = (double)Stream.Read();
            //m_selectionGlyph = Stream.Read() as IDynamicGlyph;
            //m_selectionEnvelope = Stream.Read() as IEnvelope;
            m_displayField = Stream.Read() as String;
            m_scaleSymbols = (bool)Stream.Read();
            //m_selectionSet = Stream.Read() as ISelectionSet;
            m_bufferDistance = (double)Stream.Read();
            //m_combinationMethod = (esriSelectionResultEnum)Stream.Read();
            //m_selectionColor = Stream.Read() as IColor;
            //m_selectionSymbol = Stream.Read() as ISymbol;
            //m_useSelectionSymbol = (bool)Stream.Read();
        }

        public override void Save(IVariantStream Stream)
        {
            base.Save(Stream);
            Stream.Write(m_selectable);
            Stream.Write(m_featureClassId);
            //Stream.Write(m_elements);
            //Stream.Write(m_symbolFactory);
            //Stream.Write(m_Fields);
            //Stream.Write(m_updateTimer);
            Stream.Write(m_symbolSize);
            //Stream.Write(m_selectionGlyph);
            //Stream.Write(m_selectionEnvelope);
            Stream.Write(m_displayField);
            Stream.Write(m_scaleSymbols);
            //Stream.Write(m_selectionSet);
            Stream.Write(m_bufferDistance);
            //Stream.Write(m_combinationMethod);
            //Stream.Write(m_selectionColor);
            //Stream.Write(m_selectionSymbol);
            //Stream.Write(m_useSelectionSymbol);
        }

        #endregion

        #region Drawing feature
        private void DrawFeatureClass(IDisplay display, IDynamicDisplay dynamicDisplay)
        {
            if (m_featureClass == null)
                if (m_featureClassId == -1)
                    return;
                else
                {
                    FeatureClass = Tools.GetFeatureClassFromId(m_featureClassId);
                    if (FeatureClass == null)
                        return;
                }

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

            IMxDocument mxDocument = Tools.GetMxDocument();
            IDisplayTransformation transformation = mxDocument.ActiveView.ScreenDisplay.DisplayTransformation;

            IPoint position = feature.Shape as IPoint;
            position.Project(transformation.SpatialReference);

            float factor = 0.75f * (float)transformation.FromPoints(m_symbolSize) / (float)transformation.VisibleBounds.Width;

            float zoom = factor * 1 / symbol.aspectRatio;
            properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolMarker, symbol.glyph);
            properties.SetScale(esriDynamicSymbolType.esriDSymbolMarker, zoom, zoom);
            if (IsSelected(feature))
            {
                IEnvelope envelope = position.Envelope.Envelope;
                double selectionHalfSize = transformation.FromPoints(m_symbolSize) * zoom / 2;
                envelope.Expand(selectionHalfSize, selectionHalfSize, false);
                if (m_selectionGlyph == null)
                {
                    IDynamicGlyphFactory factory = dynamicDisplay as IDynamicGlyphFactory;
                    m_selectionGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);
                }
                properties.set_DynamicGlyph(esriDynamicSymbolType.esriDSymbolLine, m_selectionGlyph);
                properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 0.6f, 0.75f, 1.0f, 0.8f);
                properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.25f, 0.5f, 0.75f, 1.0f);
                dynamicDisplay.DrawRectangle(envelope);
                if (m_selectionEnvelope != null) // $$$$ SBO 2007-07-17: debug
                {
                    properties.SetColor(esriDynamicSymbolType.esriDSymbolFill, 1.0f, 0.75f, 0.6f, 0.5f);
                    properties.SetColor(esriDynamicSymbolType.esriDSymbolLine, 0.75f, 0.5f, 0.25f, 0.8f);
                    dynamicDisplay.DrawRectangle(m_selectionEnvelope);
                }
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
            const int size = 256;
            DynamicElement element = new DynamicElement();
            element.graphic = m_symbolFactory.CreateGraphics(symbolId, "");
            element.glyph = SymbolFactory.CreateDynamicGlyph(display, dynamicDisplay, element.graphic, 256, size);

            float width = 0, height = 0;
            element.glyph.QueryDimensions(ref width, ref height);
            element.aspectRatio = height != 0 ? width / height : 1.0f;
            m_symbolSize = size; // width;
            return element;
        }
        #endregion
        
        #region Timer methods
        void SetupTimer(double msec)
        {
            m_updateTimer = new Timer(msec);
            m_updateTimer.Enabled = false;
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
                if (m_featureClass != null)
                {
                    m_featureClassId = m_featureClass.FeatureClassID;
                    m_selectable = true;
                    m_Fields.SetupTacticalElementFields(m_featureClass);
                }
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
            if (m_featureClass == null)
                return null;
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
            set { m_spatialRef = value; }
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
                IDisplayTransformation transformation = Tools.GetMxDocument().ActiveView.ScreenDisplay.DisplayTransformation;
                double symbolSize = transformation.FromPoints(m_symbolSize);
                double symbolWidth = 0.75 * symbolSize * symbolSize / (float)transformation.VisibleBounds.Width;
                double symbolHeight = 0.75 * symbolSize * symbolSize / (float)transformation.VisibleBounds.Height;

                ISpatialFilter spatialFilter = Filter as ISpatialFilter;
                IEnvelope envelope = spatialFilter.Geometry.Envelope;
                symbolWidth  = (symbolWidth > envelope.Width) ? symbolWidth - envelope.Width : envelope.Width - symbolWidth;
                symbolHeight = (symbolHeight > envelope.Height) ? symbolHeight - envelope.Height : envelope.Height - symbolHeight;
                envelope.Expand(symbolWidth / 2, symbolHeight / 2, false);
                spatialFilter.Geometry = envelope;
                m_selectionEnvelope = envelope;
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
