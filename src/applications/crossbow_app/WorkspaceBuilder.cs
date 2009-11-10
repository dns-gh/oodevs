using System;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.DataSourcesGDB;
using ESRI.ArcGIS.ArcGlobe;
using ESRI.ArcGIS.DefenseSolutions;

namespace Sword
{
    namespace Crossbow
    {
        public class WorkspaceBuilder
        {
            IFeatureWorkspace m_workspace;
            DocumentProxy m_document;
            LayersConfiguration m_layers;

            public WorkspaceBuilder(WorkspaceConfiguration config)
            {
                m_document = Tools.GetDocument();
                m_workspace = (IFeatureWorkspace)Tools.GetWorkspace(config.WorkspaceFile);
                m_layers = config.LayersConfiguration;
            }

            public bool Build()
            {
                try
                {
                    m_document.EnableDynamicDisplay(true);

                    m_document.AddLayer(BuildDynamicLayer(m_layers.Units));
                    m_document.AddLayer(BuildDynamicLayer(m_layers.KnowledgeUnits));
                    m_document.AddLayer(BuildMoleTacticalLayer(m_layers.Limits));
                    m_document.AddLayer(BuildMoleTacticalLayer(m_layers.Limas));
                    m_document.AddLayer(BuildDynamicLayer(m_layers.TacticalObjectPoint));
                    // document.RefreshView();                    
                    return true;
                }
                catch (Exception ex)
                {
                    return false;
                }
            }

            private ILayer BuildDynamicLayer(string name)
            {
                IFeatureLayer featureLayer = (IGeoFeatureLayer)BuildFeatureLayer(name);
                if (featureLayer == null)
                    return null;
                DynamicLayer dynamicLayer = new DynamicLayer(Tools.GetExtension().SymbolFactory);
                dynamicLayer.FeatureClass = featureLayer.FeatureClass;
                dynamicLayer.Connect();
                return MakeGroup(name, featureLayer, dynamicLayer);
            }

            private ILayer BuildMoleForceElementLayer(string name)
            {
                IFeatureLayer featureLayer = (IFeatureLayer)BuildFeatureLayer(name);
                if (featureLayer == null)
                    return null;
                ICachedGraphicFeatureLayer cgLayer = (ICachedGraphicFeatureLayer)new ForceElementLayerClass();
                cgLayer.FeatureLayer = featureLayer as IGeoFeatureLayer;
                return MakeGroup(name, featureLayer, cgLayer as ILayer);
            }

            private ILayer BuildMoleTacticalLayer(string name)
            {
                IFeatureLayer featureLayer = (IFeatureLayer)BuildFeatureLayer(name);
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

            private ILayer BuildFeatureLayer(string name)
            {
                IFeatureClass featureClass = m_workspace.OpenFeatureClass(name);
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
}