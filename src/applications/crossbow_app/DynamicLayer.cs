using System;
using System.Timers;
using System.Collections;
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

namespace Sword
{
    namespace Crossbow
    {
        public interface IDynamicLayerDataset
        {
            IDataset Dataset { get; }
        }

        [ComVisible(true)]
        [Guid("e5798908-98b7-466f-9eee-43e58516f700")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.DynamicLayer")]
        public sealed partial class DynamicLayer : BaseDynamicLayer, IFeatureLayer, IDynamicLayerDataset, IDisposable, IFeatureSelection, IFeatureLayerSelectionEvents_Event, IEnumFeature
        {
            private bool m_selectable = true;
            private IFeatureClass m_featureClass;
            private int m_featureClassId = -1;
            private Hashtable m_elements = new Hashtable();
            private SymbolFactory m_symbolFactory;
            private Timer m_updateTimer;
            private bool m_disposed;

            // IFeatureLayer attributes
            private String m_displayField = "name"; // $$$$ SBO 2007-07-09: Symbol_ID ?
            private bool m_scaleSymbols;

            #region class constructor/destructor
            public DynamicLayer(SymbolFactory symbolFactory)
            {                
                m_symbolFactory = symbolFactory;
                m_selectionColor = Tools.MakeColor(255, 0, 0);
                SetupTimer(500 * 10); // 2s
            }

            ~DynamicLayer()
            {
                Dispose(false);
            }

            #endregion

            #region ILayer overriden
            public override bool Valid
            {
                get
                {
                    return base.Valid && !m_disposed && m_featureClass != null;
                }
            }
            #endregion

            #region BaseDynamicLayer overriden            
            public override void DrawDynamicLayer(esriDynamicDrawPhase DynamicDrawPhase, IDisplay Display, IDynamicDisplay DynamicDisplay)
            {
                try
                {
                    if (!Valid || !Visible)
                        return;
                    if (DynamicDrawPhase != esriDynamicDrawPhase.esriDDPImmediate)
                        return;
                    
                    DrawFeatureClass(Display, DynamicDisplay);
                    // set the dirty flag to true in order to let the DynamicDisplay that the layer needs redraw.
                    // base.m_bIsImmediateDirty = true;
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.WriteLine(ex.Message);
                }
            }
            #endregion

            

            #region Drawing feature
            
            private void DrawFeatureClass(IDisplay display, IDynamicDisplay dynamicDisplay)
            {
                if (m_featureClass == null)
                    return;
                IEnvelope visibleExtent = display.DisplayTransformation.FittedBounds;
                IFeatureCursor cursor = m_featureClass.Search(null, true);
                IFeature feature = cursor.NextFeature();
                while (feature != null)
                {
                    // IEnvelope env = new EnvelopeClass() as IEnvelope;
                    if (Intersect( feature.Extent, visibleExtent))
                        DrawFeature(display, dynamicDisplay, feature);
                    feature = cursor.NextFeature();
                }
                // cursor = null;
                // GC.Collect();
            }
            
            static private bool Intersect(IEnvelope feature, IEnvelope extent)
            {
                return feature.XMin > extent.XMin && feature.XMax < extent.XMax &&
                       feature.YMin > extent.YMin && feature.YMax < extent.YMax;
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
                lock (m_elements)
                {
                    m_elements[feature.OID] = element;
                }
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
                    base.m_bIsImmediateDirty = true;
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.WriteLine(ex.Message);
                }
            }
            #endregion

            #region IFeatureLayer Members

            // If you want to obtain a selection set on a FeatureLayer it is good practice to cast the 
            // layer into an ITable then use the ITable::Select method.
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
                        IQueryFilter filter = new QueryFilterClass();
                        filter.WhereClause = "false";
                        m_selectionSet = m_featureClass.Select(filter,
                            esriSelectionType.esriSelectionTypeIDSet,
                            esriSelectionOption.esriSelectionOptionEmpty,
                            Dataset.Workspace);
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

            private void Dispose(bool usercall)
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
}