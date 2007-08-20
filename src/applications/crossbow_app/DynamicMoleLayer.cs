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

namespace Crossbow
{
    public interface IDynamicLayerDataset
    {
        IDataset Dataset { get; }
    }

    [ComVisible(true)]
    [Guid("e5798908-98b7-466f-9eee-43e58516f700")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.DynamicMoleLayer")]
    public partial class DynamicMoleLayer : BaseDynamicLayer, IFeatureLayer, IFeatureSelection, IEnumFeature, IDynamicLayerDataset, IDisposable
    {
        private bool                            m_selectable = true;
        private IFeatureClass                   m_featureClass;
        private int                             m_featureClassId = -1;
        private System.Collections.Hashtable    m_elements = new System.Collections.Hashtable();
        private SymbolFactory                   m_symbolFactory = new SymbolFactory();
        private FieldsProperty                  m_Fields = new FieldsProperty();
        private Timer                           m_updateTimer;
        private bool m_disposed;
        
        // IFeatureLayer attributes
        private String m_displayField = "name"; // $$$$ SBO 2007-07-09: Symbol_ID ?
        private bool m_scaleSymbols;
        
        #region class constructor/destructor
        public DynamicMoleLayer()
        {
            m_selectionColor = Tools.MakeColor(255, 0, 0);
            SetupTimer(100);
        }

        ~DynamicMoleLayer()
        {
            Dispose(false);
        }

        #endregion

        #region ILayer overriden
        public override void Draw(esriDrawPhase DrawPhase, IDisplay Display, ITrackCancel TrackCancel)
        {
            Tools.EnableDynamicDisplay();
        }

        public override bool Valid
        {
            get
            {
                return m_featureClass != null;
            }
        }
        #endregion

        #region BaseDynamicLayer overriden
        public override void DrawDynamicLayer(esriDynamicDrawPhase DynamicDrawPhase, IDisplay Display, IDynamicDisplay DynamicDisplay)
        {
            try
            {
                if( DynamicDisplay == null || Display == null || DynamicDrawPhase != esriDynamicDrawPhase.esriDDPImmediate )
                    return;
                if( Visible )
                    DrawFeatureClass(Display, DynamicDisplay);
            }
            catch( Exception ex )
            {
                System.Diagnostics.Trace.WriteLine(ex.Message);
            }
        }
        #endregion
        
        #region IPersistVariant overriden
        public override UID ID
        {
            get
            {
                UID id = new UIDClass();
                id.Value = "Crossbow.DynamicMoleLayer";
                return id;
            }
        }

        public override void Load(IVariantStream Stream)
        {
            if (Stream == null)
                return;
            base.Load(Stream);
            m_extensions = new System.Collections.ArrayList();
            int count = (int)Stream.Read();
            for (int i = 0; i < count; ++i)
                m_extensions.Add( Stream.Read() );

            m_selectable = (bool)Stream.Read();
            m_featureClassId = (int)Stream.Read(); m_featureClass = null;
            m_displayField = Stream.Read() as String;
            m_scaleSymbols = (bool)Stream.Read();
            m_bufferDistance = (double)Stream.Read();
        }

        public override void Save(IVariantStream Stream)
        {
            if (Stream == null)
                return;
            base.Save(Stream);
            Stream.Write(m_extensions.Count);
            for( int i = 0; i < m_extensions.Count; ++i )
                Stream.Write(m_extensions[i]);

            Stream.Write(m_selectable);
            Stream.Write(m_featureClassId);
            Stream.Write(m_displayField);
            Stream.Write(m_scaleSymbols);
            Stream.Write(m_bufferDistance);
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
            cursor = null;
        }

        private void DrawFeature(IDisplay display, IDynamicDisplay dynamicDisplay, IFeature feature)
        {
            IDynamicElement symbol = GetSymbol(display, dynamicDisplay, feature);
            if (symbol == null)
                return;
            FeatureDrawer.Draw(dynamicDisplay, symbol, feature, IsSelected(feature));
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
            string symbolId = Tools.GetValue<string>(feature, "Symbol_ID");
            IDynamicElement element = (IDynamicElement)m_elements[feature.OID];
            if (element != null)
                return element;
            if (m_symbolFactory.SpatialReference == null)
                m_symbolFactory.SpatialReference = display.DisplayTransformation.SpatialReference;
            element = m_symbolFactory.CreateElement(display, dynamicDisplay, feature, symbolId); // throw something
            m_elements[feature.OID] = element;
            return element;
        }
        #endregion

        #region Timer methods
        void SetupTimer(double msec)
        {
            m_updateTimer = new Timer(msec);
            m_updateTimer.Elapsed += new ElapsedEventHandler(OnLayerUpdateEvent);
        }

        public void Connect()
        {
            if (m_updateTimer != null)
                m_updateTimer.Start();
        }

        public void Disconnect()
        {
            if (m_updateTimer != null)
                m_updateTimer.Stop();
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

        #region IDisposable Members

        public override void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool usercall)
        {
            if (!m_disposed)
            {
                if (usercall)
                    m_updateTimer.Dispose();
                m_disposed = true;
            }
            base.Dispose();
        }

        #endregion
    }
}