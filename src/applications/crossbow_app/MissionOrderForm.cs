using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    public interface IMissionObserver
    {
        void NotifyCreated(IOrder order);
        void NotifyUpdated(IOrder order, IOrderParameter param);
        void NotifyDeleted(IOrder order);
    }

    public partial class MissionOrderForm : Form, IMissionObserver
    {
        private IController m_controller;
        
        public MissionOrderForm(IController controller)
        {
            m_controller = controller;
            InitializeComponent();            
            controller.Register(this);
        }

        #region IMissionForm implementation     
        public string OrderName
        {
            get
            {
                return base.Text;
            }

            set
            {
                base.Text = value;
            }
        }
        #endregion

        public string UnitName
        {
            get
            {
                return m_UnitName.Text;
            }

            set
            {
                m_UnitName.Text = value;
            }
        }        

        #region IMissionObserver Members
        public void NotifyCreated(IOrder order)
        {
            OrderName = order.Name;
            m_UnitName.Text = order.Target;
            if (!Visible)
                Show();
        }

        #region Visitors
        sealed class LimitsVisitor : Visitor_ABC
        {
            TextBox m_limit1;
            TextBox m_limit2;

            public LimitsVisitor(TextBox limit1, TextBox limit2) 
            {
                m_limit1 = limit1;
                m_limit1.Text = "";
                m_limit2 = limit2;
                m_limit2.Text = "";
            }
            public override void Accept(string key, string value, object img)
            {
                if (m_limit1.Text == "")
                    m_limit1.Text = value;
                else
                    m_limit2.Text = value;
            }            
        }
        sealed class LimasVisitor : Visitor_ABC
        {
            System.Windows.Forms.TreeNode m_nodes;
            public LimasVisitor(TreeNodeCollection nodes, string name)
            {
                System.Windows.Forms.TreeNode[] node = nodes.Find(name, true);
                if (node.Length == 0)
                    m_nodes = nodes.Add(name);
                else
                    m_nodes = node[0];
                
            }
            public override void Accept(string key, string value, object img)
            {                
                System.Windows.Forms.TreeNode[] node = m_nodes.Nodes.Find(key, false);
                if (node.Length != 0)
                    m_nodes.Nodes.Remove(node[0]);
                m_nodes.Nodes.Add(key, value);                
            }
        }
        sealed class ParameterVisitor : Visitor_ABC
        {
            TreeNodeCollection m_nodes;
            public ParameterVisitor(TreeNodeCollection nodes)
            {
                m_nodes = nodes;
            }
            public override void Accept(string key, string value, object img)
            {
                System.Windows.Forms.TreeNode[] node = m_nodes.Find(key, false);
                if (node.Length == 0)
                    m_nodes.Add(key, value);
                // 
            }
        }

        /*
         * Build the right visitor accord to param type
         */
        Visitor_ABC GetVisitor(IOrderParameter param)
        {
            if (param is ParameterLimits)
                return new LimitsVisitor(m_Limit1, m_Limit2);
            if (param is ParameterLimas)
                return new LimasVisitor(m_ParameterTree.Nodes, param.Name);
            return new ParameterVisitor(m_ParameterTree.Nodes);
        }
        #endregion

        public void NotifyUpdated(IOrder order, IOrderParameter param)
        {
            Visitor_ABC visitor = GetVisitor(param);
            param.Visit(visitor);
        }

        public void NotifyDeleted(IOrder order)
        {
            Hide();            
        }
        #endregion
    }
}