using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    static class Tools
    {
        static private ESRI.ArcGIS.Framework.IApplication m_application = null;
        static private bool m_dynamicDisplayEnabled = false;

        #region "Initialization"
        public static void Initialize(ESRI.ArcGIS.Framework.IApplication application)
        {
            if (application != null)
                m_application = application;
        }
        #endregion

        #region "Enable dynamic display"
        public static void EnableDynamicDisplay()
        {
            if (m_dynamicDisplayEnabled)
                return;
            IMxDocument mxDocument = Tools.GetMxDocument();
            IDynamicMap map = mxDocument.FocusMap as IDynamicMap;
            if (map == null)
                throw new System.Exception("Dynamic display not supported");
            if (!map.DynamicMapEnabled)
            {
                map.DynamicMapEnabled = true;
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
        #endregion

        #region Get CSword Extension
        public static CSwordExtension GetCSwordExtension()
        {
            if (m_application == null)
                return null;
            return (CSwordExtension)m_application.FindExtensionByName("crossbow.CSwordExtension");
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
        public static IFeatureLayer GetIFeatureLayerFromLayerName(ESRI.ArcGIS.Carto.IActiveView activeView, System.String name)
        {
            if (activeView == null || name == null)
                return null;
            int index = GetIndexNumberOfLayerName(activeView, name);
            if( index < 0 )
                return null;
            IMap map = activeView.FocusMap;
            ILayer layer = map.get_Layer( index );
            if (layer is IFeatureLayer)
                return layer as IFeatureLayer;
            else if (layer is ICompositeLayer)
            {
                ICompositeLayer composite = layer as ICompositeLayer;
                for (int i = 0; i < composite.Count; ++i)
                {
                    layer = composite.get_Layer(i);
                    if (layer is IFeatureLayer)
                        return layer as IFeatureLayer;
                }
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
                if (layer is IFeatureLayer)
                {
                    IFeatureLayer featureLayer = layer as IFeatureLayer;
                    if (featureLayer.FeatureClass.FeatureClassID == featureClassId)
                        return featureLayer.FeatureClass;
                }
                else if (layer is ICompositeLayer)
                {
                    ICompositeLayer composite = layer as ICompositeLayer;
                    for (int j = 0; j < composite.Count; ++j)
                    {
                        IFeatureLayer featureLayer = composite.get_Layer(j) as IFeatureLayer;
                        if (featureLayer != null && featureLayer.FeatureClass != null && featureLayer.FeatureClass.FeatureClassID == featureClassId)
                            return featureLayer.FeatureClass;
                    }
                }
            }
            return null;
        }
        #endregion

        #region "Retrieve Current Workspace"
        public static IFeatureWorkspace RetrieveWorkspace(IFeatureLayer pLayer)
        {            
            if (pLayer != null)
            {                
                IDataset pDataset = null;
                if ( pLayer is IDynamicLayerDataset )
                {
                    IDynamicLayerDataset dataset = pLayer as IDynamicLayerDataset;
                    pDataset = dataset.Dataset;
                }
                else
                    pDataset = (IDataset)pLayer;
                if ( pDataset != null )
                    return (IFeatureWorkspace)pDataset.Workspace;
            }
            return null;
        }
        
        public static IFeatureWorkspace OpenWorkspace(ESRI.ArcGIS.Carto.IActiveView activeView, System.String name)
        {
            return RetrieveWorkspace(Tools.GetIFeatureLayerFromLayerName(activeView, name));            
        }
        #endregion

        #region "Get value of specified feature field"
        public static string GetFeatureValue(IFeature feature, string fieldName)
        {
            int index = feature.Fields.FindField(fieldName);
            if (index < 0 )
                return null;
            return feature.get_Value(index) as string;
        }

        public static void SetValue<T>(IRow row, string field, T value)
        {
            int id = row.Fields.FindField(field);
            if (id >= 0 && row.Fields.get_Field(id).Editable)
                row.set_Value(id, value);
        }

        public static T GetValue<T>(IRow row, string field)
        {
            int id = row.Fields.FindField(field);
            if (id >= 0)
                return (T)row.get_Value(id);
            return default(T);
        }
        #endregion

        #region "Create IColor from RGB components"
        public static ESRI.ArcGIS.Display.IColor MakeColor(short red, short green, short blue)
        {
            ESRI.ArcGIS.Display.IRgbColor color = new ESRI.ArcGIS.Display.RgbColorClass();
            color.Red = red;
            color.Green = green;
            color.Blue = blue;
            return color as ESRI.ArcGIS.Display.IColor;
        }
        #endregion
    }
}