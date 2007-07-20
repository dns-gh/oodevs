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
    public partial class DynamicMoleLayer : BaseDynamicLayer, IFeatureLayer, IFeatureSelection, IEnumFeature, IDynamicLayerDataset
    {
        private class DynamicElement : IDynamicElement
        {
            private ICachedGraphic graphic;
            private IDynamicGlyph glyph;
            private float aspectRatio; // glyph.width / glyph.height

            public ICachedGraphic CachedGraphic
            {
                get
                {
                    return graphic;
                }
                set 
                {
                    graphic = value;
                }
            }
            public IDynamicGlyph Glyph
            {
                get
                {
                    return glyph;
                }
                set
                {
                    glyph = value;
                }
            }
            public float Ratio
            {
                get
                {
                    return aspectRatio;
                }
                set
                {
                    aspectRatio = value;
                }
            }
        };

        private bool                            m_selectable = true;
        private IFeatureClass                   m_featureClass = null;
        private int                             m_featureClassId = -1;
        private System.Collections.Hashtable    m_elements = new System.Collections.Hashtable();
        private SymbolFactory                   m_symbolFactory = new SymbolFactory();
	    private OrderManager 			        m_orderManager = null;
        private FieldsProperty                  m_Fields = new FieldsProperty();
        private Timer                           m_updateTimer;
        private double                          m_symbolSize = 0;
        private IDynamicGlyph                   m_selectionGlyph = null;
        private IEnvelope                       m_selectionEnvelope = null;
        
        // IFeatureLayer attributes
        private String m_displayField = "name"; // $$$$ SBO 2007-07-09: Symbol_ID ?
        private bool m_scaleSymbols = false;
        
        private IDocumentEvents_OnContextMenuEventHandler m_ContextMenu;

        #region class constructor/destructor
        public DynamicMoleLayer()
            : base()
        {
            m_selectionColor = Tools.MakeColor(255, 0, 0);
            SetupTimer(100);
        }

        ~DynamicMoleLayer()
        {
            Disconnect();
        }
        #endregion

        public void Initialize(IApplication app)
        {
            CSwordExtension extension = Tools.GetCSwordExtension(app);
            m_orderManager = extension.OrderManager;
            SetupEvents();
        }

        #region BaseDynamicLayer overriden
        void ILayer.Draw(esriDrawPhase DrawPhase, IDisplay Display, ITrackCancel TrackCancel)
        {
            Tools.EnableDynamicDisplay();
        }

        private void SetupEvents()
        {
            IMxDocument mxDocument = Tools.GetMxDocument();
            m_ContextMenu = new IDocumentEvents_OnContextMenuEventHandler(OnContextMenuEvent);
            ((IDocumentEvents_Event)mxDocument).OnContextMenu += m_ContextMenu;
        }

        private void OnContextMenuEvent(int x, int y, out bool result)
        {
            if (m_selectionSet.Count >= 1)
            {
                m_orderManager.OpenContextMenu(this, x, y);
                result = true;
            }
            else
                result = false;
        }
        #endregion

        #region BaseDynamicLayer overriden
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
       
        public IDynamicGlyph UpdateGlyphSelection(IDynamicGlyphFactory factory)
        {
            if (m_selectionGlyph == null)                            
                m_selectionGlyph = factory.get_DynamicGlyph(1, esriDynamicGlyphType.esriDGlyphLine, 8);            
            return m_selectionGlyph;
        }
        public IEnvelope Enveloppe
        {
            get
            {
                return m_selectionEnvelope;
            }            
        }

        private void DrawFeature(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            IDynamicElement symbol = GetSymbol(display, dynamicDisplay, feature);
            if (symbol == null)
                return;
            IDynamicSymbolProperties    properties = dynamicDisplay as IDynamicSymbolProperties;
            IMxDocument mxDocument = Tools.GetMxDocument();            
            IDisplayTransformation      transformation = mxDocument.ActiveView.ScreenDisplay.DisplayTransformation;
            
            feature.Shape.Project(transformation.SpatialReference);

            FeatureDrawer_ABC drawer = FeatureDrawerFactory.Create(this, feature);
            float factor = 0.75f * (float)transformation.FromPoints(m_symbolSize) / (float)transformation.VisibleBounds.Width;
            if ( drawer != null )
                drawer.InitializeDisplay(symbol, factor, properties);            
            if ( IsSelected(feature) )
                drawer.DrawEnvelope(dynamicDisplay,             transformation.FromPoints(m_symbolSize), properties);
            drawer.Draw(dynamicDisplay, m_Fields);
            properties.SetColor(esriDynamicSymbolType.esriDSymbolMarker, 1, 1, 1, 1);
        }

        private bool IsSelected(IFeature feature)
        {
            if (m_selectionSet == null)
                return false;            
            IEnumIDs ids = m_selectionSet.IDs;
            ids.Reset();
            int id = ids.Next();
            while (id != -1)
            {
                if (id == feature.OID)
                    return true;
                id = ids.Next();
            }
            return false;
        }

        private IDynamicElement GetSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            string symbolId = Tools.GetFeatureValue(feature, "Symbol_ID");
            IDynamicElement element = (IDynamicElement)m_elements[symbolId];
            if (element != null)
                return element;
            if (m_symbolFactory.SpatialReference == null)
                m_symbolFactory.SpatialReference = display.DisplayTransformation.SpatialReference;
            element = BuildSymbol(display, dynamicDisplay, symbolId); // throw something
            m_elements[symbolId] = element;
            return element;
        }

        private IDynamicElement BuildSymbol(IDisplay display, IDynamicDisplay dynamicDisplay, String symbolId)
        {
            const int size = 256;
            DynamicElement element = new DynamicElement();
            element.CachedGraphic = m_symbolFactory.CreateGraphics(symbolId, "");
            element.Glyph = SymbolFactory.CreateDynamicGlyph(display, dynamicDisplay, element.CachedGraphic, 256, size);

            float width = 0, height = 0;
            element.Glyph.QueryDimensions(ref width, ref height);
            element.Ratio = height != 0 ? width / height : 1.0f;
            m_symbolSize = size; // width;
            return (IDynamicElement)element;
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
    }
}