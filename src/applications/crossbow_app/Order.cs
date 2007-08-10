using System.Collections.Generic;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public interface IOrder 
    {
        string Name { get; }
        string Target { get; }
        int OID { get; }
        void RegisterParameter(IOrderParameter parameter);
        void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected);
        void Validate();
        void Release();
    }

    public class Order : IOrder
    {        
        private ParameterLimits m_limits = new ParameterLimits();
        private ParameterLimas m_limas = new ParameterLimas();
        private ParameterDirection m_direction = new ParameterDirection();
        private Dictionary<string, IOrderParameter> m_parameters = new Dictionary<string, IOrderParameter>();        
        private int m_OID;
        private string m_name;
        private string m_target;

        private IMissionObserver m_observer;
        private OrderHandler m_handler;
        private IFeature m_feature;
        public string Name
        {
            get
            {
                return m_name;
            }
        }
        public string Target
        {
            get
            {
                return m_target;
            }
        }
        public int OID
        {
            get
            {
                return m_OID;
            }
            set
            {
                m_OID = value;
            }
        }

        public Order(string name, OrderHandler handler, IParameterFactory factory)
        {
            m_handler = handler;
            m_feature = handler.TargetFeature;
            m_name = name;
            m_OID = Tools.GetValue<int>(m_feature, "Public_OID");
            m_target = Tools.GetValue<string>(m_feature, "Name");            
            m_observer = new MissionOrderForm(this);
            factory.CreateParameters(this);            
            m_handler.Register(this);
        }

        public void RegisterParameter(IOrderParameter parameter)
        {
            m_parameters.Add(parameter.Name, parameter);
            parameter.NotifyUpdate(m_observer);
        }

        public void SelectParameter(IOrderParameter parameter, string value)
        {
            System.Console.Write("SelectParameter[ " + parameter.ToString() + "]:" + value);
            parameter.SetValue(value);
            parameter.NotifyUpdate(m_observer);
        }

        public void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected)
        {            
            m_limits.OnContextMenu(menu, x, y, selected);
            m_limas.OnContextMenu(menu, x, y, selected);
            foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                param.Value.OnContextMenu(menu, x, y, selected);
        }

        /*
         * Enable IFeatureWorkspace edition during instance life
         */
        class ScopeLockEditor
        {
            IWorkspaceEdit m_edit;

            public ScopeLockEditor(IFeatureWorkspace featureWorkspace)
            {
                m_edit = (IWorkspaceEdit)featureWorkspace;                
            }
            public void Lock()
            {
                m_edit.StartEditing(false);
                m_edit.StartEditOperation();
            }
            public void Unlock()
            {
                m_edit.StopEditOperation();
                m_edit.StopEditing(true);
            }
        }

        public void Validate()
        {
            IFeatureWorkspace workspace = Tools.RetrieveWorkspace(m_feature.Table);
            if (workspace != null)
                Serialize(workspace);
        }

        public void Release()
        {
            m_handler.Release();
        }

        public void Serialize(IFeatureWorkspace featureWorkspace)
        {
            // ScopeLockEditor editor = new ScopeLockEditor(featureWorkspace);

            IWorkspace wks = (IWorkspace)featureWorkspace;
            System.Console.Write(wks.PathName);
            // editor.Lock();
            int id = SerializeOrder(featureWorkspace);
            SerializeParameters(featureWorkspace, id);
            // editor.Unlock();
        }

        int SerializeOrder(IFeatureWorkspace featureWorkspace)
        {
            ITable table = featureWorkspace.OpenTable("Orders"); // $$$$ SBO 2007-07-20: keep it maybe...                
            IRow row = table.CreateRow();
            Tools.SetValue<int>(row, "target_id", m_OID);
            Tools.SetValue<string>(row, "OrderName", m_name);
            Tools.SetValue<bool>(row, "processed", false);
            row.Store();
            return row.OID;
        }

        void SerializeParameters(IFeatureWorkspace featureWorkspace, int orderId)
        {
            ITable table = featureWorkspace.OpenTable("OrdersParameters"); // $$$$ SBO 2007-07-20: keep it maybe...

            m_limits.Serialize(table, orderId);
            m_limas.Serialize(table, orderId);
            m_direction.Serialize(table, orderId);
            foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                param.Value.Serialize(table, orderId);            
        }
    }
}
