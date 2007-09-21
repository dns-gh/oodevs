using System;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.MOLE;
using ESRI.ArcGIS.DataSourcesGDB;

namespace Crossbow
{
    public static class WorkspaceBuilder
    {
        public static void Build(WorkspaceConfiguration config)
        {
            IWorkspaceFactory factory = new AccessWorkspaceFactoryClass();
            IFeatureWorkspace workspace = (IFeatureWorkspace)factory.OpenFromFile(config.WorkspaceFile, 0);

            IMxDocument document = Tools.GetMxDocument();
            document.AddLayer(BuildDynamicLayer(workspace, config.LayersConfiguration.Units));
            document.AddLayer(BuildMoleTacticalLayer(workspace, config.LayersConfiguration.Limits));
            document.AddLayer(BuildMoleTacticalLayer(workspace, config.LayersConfiguration.Limas));
            document.AddLayer(BuildDynamicLayer(workspace, config.LayersConfiguration.TacticalObjectPoint));
            document.AddLayer(BuildDynamicLayer(workspace, config.LayersConfiguration.Emergencies));
            document.ActiveView.Refresh();
        }

        private static ILayer BuildDynamicLayer(IFeatureWorkspace workspace, string name)
        {
            IFeatureLayer featureLayer = (IGeoFeatureLayer)BuildFeatureLayer(workspace, name);
            if (featureLayer == null)
                return null;
            DynamicMoleLayer dynamicLayer = new DynamicMoleLayer(Tools.GetCSwordExtension().SymbolFactory);
            dynamicLayer.FeatureClass = featureLayer.FeatureClass;
            dynamicLayer.Connect();
            return MakeGroup(name, featureLayer, dynamicLayer);
        }

        private static ILayer BuildMoleForceElementLayer(IFeatureWorkspace workspace, string name)
        {
            IFeatureLayer featureLayer = (IFeatureLayer)BuildFeatureLayer(workspace, name);
            if (featureLayer == null)
                return null;
            ICachedGraphicFeatureLayer cgLayer = (ICachedGraphicFeatureLayer)new ForceElementLayerClass();
            cgLayer.FeatureLayer = featureLayer as IGeoFeatureLayer;
            return MakeGroup(name, featureLayer, cgLayer as ILayer);
        }

        private static ILayer BuildMoleTacticalLayer(IFeatureWorkspace workspace, string name)
        {
            IFeatureLayer featureLayer = (IFeatureLayer)BuildFeatureLayer(workspace, name);
            if (featureLayer == null)
                return null;
            ICachedGraphicFeatureLayer cgLayer = (ICachedGraphicFeatureLayer)new TacticalGraphicLayerClass();
            cgLayer.FeatureLayer = featureLayer as IGeoFeatureLayer;

            ITacticalGraphicLayer tgLayer = (ITacticalGraphicLayer)cgLayer;
            tgLayer.TextSize = 0.006;
            tgLayer.UseAffiliationColor = true;

            ICacheRendererCollection renderers = (ICacheRendererCollection)cgLayer;
            ICacheRenderer renderer = new TacticalGraphic2525BRendererClass();
            renderers.AddRenderer(ref renderer, 0);
            return MakeGroup(name, featureLayer, cgLayer as ILayer);
        }

        private static ILayer BuildFeatureLayer(IFeatureWorkspace workspace, string name)
        {
            IFeatureClass featureClass = workspace.OpenFeatureClass(name);
            if (featureClass == null)
                return null;
            IFeatureLayer layer = new FeatureLayerClass();
            layer.Name = name;
            layer.FeatureClass = featureClass;
            return layer as ILayer;
        }

        private static ILayer MakeGroup(string name, ILayer featureLayer, ILayer graphicLayer)
        {
            IGroupLayer group = new GroupLayerClass();
            ILayer layer = group as ILayer;
            layer.Name = name;
            featureLayer.Name = name + " - Features";
            graphicLayer.Name = name + " - Graphics";
            group.Add(featureLayer);
            group.Add(graphicLayer);
            return group as ILayer;
        }
    }
}