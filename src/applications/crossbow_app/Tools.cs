using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MilitaryAnalyst;

namespace Crossbow
{
    static class Tools
    {
        static private ESRI.ArcGIS.Framework.IApplication m_application;
        static private bool m_dynamicDisplayEnabled;

        #region "Initialization"
        public static void Initialize(IApplication application)
        {
            if (application != null)
                m_application = application;
        }
        #endregion

        #region "Enable dynamic display"
        public static void EnableDynamicDisplay(bool enable)
        {
            if (m_dynamicDisplayEnabled == enable)
                return;
            IMxDocument mxDocument = Tools.GetMxDocument();
            IDynamicMap map = mxDocument.FocusMap as IDynamicMap;
            if (map == null)
                throw new System.Exception("Dynamic display not supported");
            if (map.DynamicMapEnabled != enable)
            {
                map.DynamicMapEnabled = enable;
                mxDocument.ActiveView.Refresh();
            }
            m_dynamicDisplayEnabled = map.DynamicMapEnabled;
        }
        #endregion

        #region "Get MxDocument"
        public static IMxDocument GetMxDocument()
        {
            if (m_application == null)
                return null;
            return (IMxDocument)m_application.Document;
        }
        public static IAppDisplay GetDisplay()
        {
            if (m_application == null)
                return null;
            return ((IMxApplication)m_application).Display;
        }
        #endregion

        #region "Get CSword Extension"
        public static CSwordExtension GetCSwordExtension()
        {
            if (m_application == null)
                return null;
            return (CSwordExtension)m_application.FindExtensionByName("Crossbow.CSwordExtension");
        }
        #endregion

        #region "Get Index Number from Layer Name"
        // ArcGIS Snippet Title: 
        // Get Index Number from Layer Name
        //
        // Add the following references to the project:
        // ESRI.ArcGIS.Carto
        // 
        // Intended ArcGIS Products for this snippet:
        // ArcGIS Desktop
        // ArcGIS Engine
        // ArcGIS Server
        //
        // Required ArcGIS Extensions:
        // (NONE)
        //
        // Notes:
        // This snippet is intended to be inserted at the base level of a Class.
        // It is not intended to be nested within an existing Method.
        //
        // Use the following XML documentation comments to use this snippet:
        /// <summary>Get the index number for the specified layer name.</summary>
        ///
        /// <param name="activeView">An IActiveView interface</param>
        /// <param name="layerName">A System.String that is the layer name in the active view. Example: "states"</param>
        /// 
        /// <returns>An System.Int32 representing a layer number</returns>
        /// 
        /// <remarks>Returns a System.Int32 where values of 0 and greater are valid layers. A return value of -1 means the layer name was not found.</remarks>
        public static System.Int32 GetIndexNumberOfLayerName(ESRI.ArcGIS.Carto.IActiveView activeView, System.String layerName)
        {
            if (activeView == null || layerName == null)
            {
                return -1;
            }
            ESRI.ArcGIS.Carto.IMap map = activeView.FocusMap;

            // Get the number of layers
            int numberOfLayers = map.LayerCount;

            // Loop through the layers and get the correct layer index
            for (System.Int32 i = 0; i < numberOfLayers; i++)
            {
                if (layerName == map.get_Layer(i).Name)
                {
                    // Layer was found
                    return i;
                }
            }

            // No layer was found
            return -1;
        }
        #endregion
                
        #region "Get FeatureLayer from Layer Name"
        public static IFeatureLayer GetIFeatureLayerFromLayerName(string name)
        {
            IMxDocument doc = GetMxDocument();
            IActiveView activeView = doc.ActiveView;
            if (activeView == null || name == null)
                return null;
            int index = GetIndexNumberOfLayerName(activeView, name);
            if( index < 0 )
                return null;
            IMap map = activeView.FocusMap;
            ILayer layer = map.get_Layer( index );

            IFeatureLayer featureLayer = layer as IFeatureLayer;
            if (featureLayer != null)
                return featureLayer;

            ICompositeLayer compositeLayer = layer as ICompositeLayer;
            if (compositeLayer != null)
                for (int i = 0; i < compositeLayer.Count; ++i)
                {
                    featureLayer = compositeLayer.get_Layer(i) as IFeatureLayer;
                    if (featureLayer != null)
                        return featureLayer;
                }
            return null;
        }
        #endregion

        #region "Get FeatureClass from Class Id"
        public static IFeatureClass GetFeatureClassFromId(int featureClassId)
        {
            ESRI.ArcGIS.Carto.IActiveView activeView = GetMxDocument().ActiveView;
            if (activeView == null)
                return null;
            IMap map = activeView.FocusMap;
            for (int i = 0; i < map.LayerCount; ++i)
            {
                ILayer layer = map.get_Layer(i);
                IFeatureLayer featureLayer = layer as IFeatureLayer;
                if (featureLayer != null)
                {
                    if (featureLayer.FeatureClass.FeatureClassID == featureClassId)
                        return featureLayer.FeatureClass;
                    continue;
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

        public static IFeatureWorkspace OpenWorkspace(string name)
        {
            return RetrieveWorkspace(Tools.GetIFeatureLayerFromLayerName(name));            
        }
        #endregion

        #region "Data manipulation"
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
                System.Console.Write(e.Message);
            }
        }

        public static T GetValue<T>(IRow row, string field)
        {
            int id = row.Fields.FindField(field);
            if (id >= 0)
                return (T)row.get_Value(id);
            return default(T);
        }

        public static void Store(string table, IGeometry value)
        {
            try
            {
                IFeatureWorkspace ws = OpenWorkspace(Tools.GetCSwordExtension().Config.LayersConfiguration.Units);
                IFeatureClass features = ws.OpenFeatureClass(table);
                IFeature feature = features.CreateFeature();
                feature.Shape = value;
                feature.Store();
            }
            catch (System.Exception e)
            {
                System.Console.Write(e.Message);
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
            IDisplay display = GetDisplay();
            IPoint point = GetDisplay().DisplayTransformation.ToMapPoint(x, y);            
            point.Project(display.DisplayTransformation.SpatialReference);    
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
                System.Console.WriteLine(e.Message);
            }
            return mgrs;
        }
        #endregion
    }
}