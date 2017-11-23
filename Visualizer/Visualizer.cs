using System.Windows.Forms;
using CefSharp;
using CefSharp.WinForms;

namespace Visualizer
{
    public partial class Visualizer : Form
    {
        private readonly ChromiumWebBrowser chromium;

        public Visualizer()
        {
            InitializeComponent();
            chromium = new ChromiumWebBrowser("file://C:\\Users\\hp\\Desktop\\index.html")
            {
                Dock = DockStyle.Fill,
                BrowserSettings = new BrowserSettings()
                {
                    FileAccessFromFileUrls = CefState.Enabled,
                    UniversalAccessFromFileUrls = CefState.Enabled
                }
            };

            tableLayoutPanel1.Controls.Add(chromium, 1, 0);

            ClientSizeChanged += (sender, args) => { chromium.Reload(); };
        }
    }
}