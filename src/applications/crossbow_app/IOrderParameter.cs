using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public abstract class Visitor_ABC
    {        
        public abstract void Accept(string key, string value, object img);
    }

    public interface IOrderParameter
    {
        string Name { get; }
        void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected);
        void SetValue(string value);
        void Visit(Visitor_ABC visitor);
        void Serialize(ITable table, int oid);
    }
}
