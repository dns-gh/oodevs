using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.DataSourcesGDB;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MilitaryAnalyst;
using ESRI.ArcGIS.ArcGlobe;
using ESRI.ArcGIS.Analyst3D;

namespace Crossbow
{
    static class Tools
    {
        static private IApplication m_application;
        static private DocumentProxy m_document;

        #region "Initialization"
        public static void Initialize(IApplication application)
        {
            if (application != null)
                m_application = application;
        }
        #endregion

        #region Get Document
        public static IDocument GetIDocument()
        {
            if (m_application != null)
                return m_application.Document;
            return null;
        }

        public static DocumentProxy GetDocument()
        {
            if (m_application == null)
                return null;
            if (m_document == null)
                m_document = new DocumentProxy(m_application.Document);
            return m_document;
        }
        #endregion

        public static bool IsSupportedApplication(object hook)
        {
            return hook != null && (hook is IMxApplication || hook is IGMxApplication);
        }

        #region "Get CSword Extension"
        public static CSwordExtension GetCSwordExtension()
        {
            if (m_application == null)
                return null;
            return (CSwordExtension)m_application.FindExtensionByName("Crossbow.CSwordExtension");
        }
        #endregion

        #region "Get FeatureLayer by Name"
        public static IFeatureLayer GetFeatureLayerByName(string name)
        {
            DocumentProxy layers = GetDocument();
            for (int i = 0; i < layers.LayerCount; ++i)
            {
                IFeatureLayer layer = layers.GetLayer(i) as IFeatureLayer;
                if (layer != null)
                {
                    if (layer.Name == name)
                        return layer;
                }
                else
                {
                    ICompositeLayer composite = layers.GetLayer(i) as ICompositeLayer;
                    if (composite != null)
                        for (int j = 0; j < composite.Count; ++j)
                        {
                            layer = composite.get_Layer(j) as IFeatureLayer;
                            if (layer != null && layer.Name == name + " - Features")
                                return layer;
                        }
                }
            }
            return null;
        }
        #endregion

        #region "Get Layer by name"
        public static ILayer GetLayerByName(string name)
        {
            DocumentProxy layers = GetDocument();
            for (int i = 0; i < layers.LayerCount; ++i)
            {
                ILayer layer = layers.GetLayer(i);
                if (layer != null && layer.Name == name)
                    return layer;
                ICompositeLayer composite = layer as ICompositeLayer;
                if (composite != null)
                    for (int j = 0; j < composite.Count; ++j)
                    {
                        layer = composite.get_Layer(j);
                        if (layer.Name == name)
                            return layer;
                    }
            }
            return null;
        }
        #endregion

        #region "Get FeatureClass from Class Id"
        public static IFeatureClass GetFeatureClassFromId(int featureClassId)
        {
            DocumentProxy layers = GetDocument();
            for (int i = 0; i < layers.LayerCount; ++i)
            {
                ILayer layer = layers.GetLayer(i);
                IFeatureLayer featureLayer = layer as IFeatureLayer;
                if (featureLayer != null)
                {
                    if (featureLayer.FeatureClass.FeatureClassID == featureClassId)
                        return featureLayer.FeatureClass;
                }
                else
                {
                    ICompositeLayer composite = layer as ICompositeLayer;
                    if (composite != null)
                        for (int j = 0; j < composite.Count; ++j)
                        {
                            featureLayer = composite.get_Layer(j) as IFeatureLayer;
                            if (featureLayer != null && featureLayer.FeatureClass != null && featureLayer.FeatureClass.FeatureClassID == featureClassId)
                                return featureLayer.FeatureClass;
                        }
                }
            }
            return null;
        }
        #endregion

        #region "Retrieve Current Workspace"
        public static IFeatureWorkspace RetrieveWorkspace(IFeatureLayer layer)
        {
            if (layer == null)
                return null;
            IDynamicLayerDataset dynamicLayerDataset = layer as IDynamicLayerDataset;
            if (dynamicLayerDataset != null)
                return (IFeatureWorkspace)dynamicLayerDataset.Dataset.Workspace;
            return (IFeatureWorkspace)((IDataset)layer).Workspace;
        }

        public static IFeatureWorkspace RetrieveWorkspace(ITable table)
        {            
            IDataset dataset = table as IDataset;
            if (dataset != null)
                return (IFeatureWorkspace)dataset.Workspace;
            return null;
        }

        public static IFeatureWorkspace GetWorkspace(string name)
        {
            return RetrieveWorkspace(Tools.GetFeatureLayerByName(name));            
        }

        ///<summary>
        /// Open a valid workspace according to the database's extension
        ///</summary>
        ///<param name="file">Name of the workspace</param>
        public static IWorkspace OpenWorkspace(string file)
        {
            IWorkspaceFactory factory = null;
            string ext = file.Substring(file.LastIndexOf('.') + 1, 3);
            if (ext == "mdb")
                factory = new AccessWorkspaceFactoryClass();
            if (ext == "gdb")
                factory = new FileGDBWorkspaceFactoryClass();
            if (factory != null)
                return factory.OpenFromFile(file, 0);
            return null;
        }
        #endregion

        #region "Data manipulation"
        public static void SetValue<T>(IRowBuffer row, string field, T value)
        {
            try
            {
                int id = row.Fields.FindField(field);
                if (id >= 0 && row.Fields.get_Field(id).Editable)
                    row.set_Value(id, value);
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.Message);
            }
        }

        public static void SetValue<T>(IRow row, string field, T value)
        {
            try
            {
                int id = row.Fields.FindField(field);
                if (id >= 0 && row.Fields.get_Field(id).Editable)
                    row.set_Value(id, value);
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.Message);
            }
        }

        /// <summary>
        ///  Retrieve value from a specific data field
        /// </summary>
        /// <typeparam name="T">Value type</typeparam>
        /// <param name="row">row</param>
        /// <param name="field">field name</param>
        /// <returns>Value of the field</returns>
        public static T GetValue<T>(IRow row, string field)
        {
            int id = row.Fields.FindField(field);
            if (id >= 0)
                return (T)row.get_Value(id);
            return default(T);
        }

        /// <summary>
        ///  Store geometry into table. Create a new feature and store input geometry
        /// </summary>
        /// <param name="table">Name of the output table</param>
        /// <param name="value">Input geometry</param>
        /// <remarks>Input geometry should be zAwared according to the table</remarks>
        public static void Store(string table, IGeometry value)
        {
            try
            {
                IFeatureWorkspace ws = (IFeatureWorkspace)OpenWorkspace(Tools.GetCSwordExtension().Config.SharedFile);
                ScopeLockEditor locker = new ScopeLockEditor(ws);

                locker.Lock();
                IFeatureClass features = ws.OpenFeatureClass(table);                                
                IFeature feature = features.CreateFeature();         
                feature.Shape = value;
                feature.Store();
                locker.Unlock();
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.Message);
            }
        }

        /// <summary>
        ///  Remove all features of the specified feature class
        /// </summary>
        /// <param name="workspace">Workspace</param>
        /// <param name="name">Feature class</param>
        public static void ClearClass(string workspace, string name)
        {
            try
            {
                IFeatureWorkspace ws = (IFeatureWorkspace)OpenWorkspace(workspace);
                if (ws == null)
                    return;
                ITable table = (ITable)ws.OpenFeatureClass(name);
                if (table != null)
                {
                    IQueryFilter filter = null;
                    table.DeleteSearchedRows(filter);
                }
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.Message);
            }
        }
        #endregion

        #region "Create IColor from RGB components"
        public static ESRI.ArcGIS.Display.IColor MakeColor(short red, short green, short blue)
        {
            ESRI.ArcGIS.Display.IRgbColor color = new ESRI.ArcGIS.Display.RgbColorClass();
            color.Red = red;
            color.Green = green;
            color.Blue = blue;
            return (ESRI.ArcGIS.Display.IColor)color;
        }
        #endregion
    
        #region "Geometry tools"
        static public IPoint MakePoint(int x, int y)
        {
            IPoint point = GetDocument().Display.DisplayTransformation.ToMapPoint(x, y);
            point.Project(GetDocument().Display.DisplayTransformation.SpatialReference);
            return point;
        }
        static public string ConvertToMGRS(IPoint point)
        {
            ISpatialReferenceFactory    sp = new SpatialReferenceEnvironment();            
            IGeographicCoordinateSystem gcs = sp.CreateGeographicCoordinateSystem((int)esriSRGeoCSType.esriSRGeoCS_WGS1984);
            ICoordinateTool converter = new CoordinateToolClass();
            IPoint datum = new PointClass();
            IPoint wgs = new PointClass();
            string dms = "", utm = "", mgrs = "";
            try
            {
                point.Project(gcs); // Project point to WGS
                converter.ConvertLocation(point, 1, "WGS 1984 (WGS84)", "WGS 1984 (WGS84)", ref wgs, ref datum, ref dms, ref utm, ref mgrs);
            }
            catch (System.Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.Message);
            }
            return mgrs;
        }
        #endregion
    }
}